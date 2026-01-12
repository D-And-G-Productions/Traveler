#pragma once

#include <condition_variable>
#include <cstddef>
#include <memory>
#include <mutex>
#include <pqxx/pqxx>
#include <queue>
#include <stdexcept>
#include <string>
#include <utility>

// TODO: Refactor Class. Straight AI slop.
class DBPool
{
public:
  /**
   * Lease class reclaims the connection for the dbpool on destruction,
   * providing safety should an exception occur.
   */
  class Lease
  {
  public:
    Lease() = default;

    Lease(const Lease &) = delete;
    Lease &operator=(const Lease &) = delete;

    Lease(Lease &&other) noexcept { *this = std::move(other); }

    Lease &operator=(Lease &&other) noexcept
    {
      if (this != &other)
      {
        reset();
        pool_ = other.pool_;
        conn_ = std::move(other.conn_);
        other.pool_ = nullptr;
      }
      return *this;
    }

    ~Lease() noexcept { reset(); }

    pqxx::connection &connection()
    {
      if (!conn_)
      {
        throw std::runtime_error("DBPool::Lease has no connection");
      }
      return *conn_;
    }

    const pqxx::connection &connection() const
    {
      if (!conn_)
      {
        throw std::runtime_error("DBPool::Lease has no connection");
      }
      return *conn_;
    }

    void reset() noexcept
    {
      if (pool_ && conn_)
      {
        pool_->release(std::move(conn_));
      }
      conn_.reset();
      pool_ = nullptr;
    }

    explicit operator bool() const noexcept { return conn_ != nullptr; }

  private:
    friend class DBPool;

    Lease(DBPool &pool, std::unique_ptr<pqxx::connection> conn) noexcept :
        pool_(&pool), conn_(std::move(conn))
    {
    }

    DBPool *pool_{nullptr};
    std::unique_ptr<pqxx::connection> conn_{};
  };

  DBPool(std::string connectionString, std::size_t poolSize) :
      connectionString_(std::move(connectionString)), poolSize_(poolSize)
  {
    if (poolSize_ == 0)
    {
      throw std::invalid_argument("DBPool poolSize must be > 0");
    }
    for (std::size_t i = 0; i < poolSize_; ++i)
    {
      pool_.push(makeConnection());
    }
  }

  DBPool(const DBPool &) = delete;
  DBPool &operator=(const DBPool &) = delete;

  [[nodiscard]] Lease acquire()
  {
    std::unique_ptr<pqxx::connection> conn;

    {
      std::unique_lock lock(mutex_);
      conditionVariable_.wait(lock, [&] { return !pool_.empty(); });

      conn = std::move(pool_.front());
      pool_.pop();
    }

    // If reconnect fails, put the (possibly dead) connection back so the pool size
    // does not shrink and deadlock callers.
    try
    {
      if (!conn || !conn->is_open())
      {
        conn = makeConnection();
      }
    }
    catch (...)
    {
      release(std::move(conn));
      throw;
    }

    return Lease{*this, std::move(conn)};
  }

private:
  friend class Lease;

  void release(std::unique_ptr<pqxx::connection> conn) noexcept
  {
    if (!conn)
    {
      return;
    }
    std::lock_guard lock(mutex_);
    pool_.push(std::move(conn));
    conditionVariable_.notify_one();
  }

  std::unique_ptr<pqxx::connection> makeConnection() const
  {
    auto conn = std::make_unique<pqxx::connection>(connectionString_);
    if (!conn->is_open())
    {
      throw std::runtime_error("Failed to open PostgreSQL connection");
    }
    return conn;
  }

  std::string connectionString_;
  std::size_t poolSize_{0};

  std::mutex mutex_;
  std::condition_variable conditionVariable_;
  std::queue<std::unique_ptr<pqxx::connection>> pool_;
};
