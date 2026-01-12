#include <gtest/gtest.h>
#include <memory>
#include <pqxx/pqxx>

class LibpqxxExplorationTests : public ::testing::Test
{
private:
  std::unique_ptr<pqxx::connection> connection;
  std::unique_ptr<pqxx::work> transaction_;

protected:
  pqxx::work &transaction() { return *transaction_; }

  void SetUp() override
  {
    try
    {
      const std::string url = requireEnv("DATABASE_URL");
      connection = std::make_unique<pqxx::connection>(url);

      ASSERT_TRUE(connection->is_open());
      transaction_ = std::make_unique<pqxx::work>(*connection);
    }
    catch (const std::exception &e)
    {
      GTEST_SKIP() << e.what();
    }
  }

  void TearDown() override
  {
    abortTransaction();
    resetTransactionThenConnection();
  }

  static std::string requireEnv(const char *key)
  {
    const char *variable = std::getenv(key);
    if (variable && *variable)
      return std::string{variable};
    throw std::runtime_error(std::string{"Missing env variable: "} + key);
  }

  pqxx::connection createConnection()
  {
    const std::string url = requireEnv("DATABASE_URL");
    return pqxx::connection{url};
  }

private:
  void abortTransaction()
  {
    try
    {
      if (transaction_)
        transaction_->abort();
    }
    catch (const std::exception &e)
    {
      ADD_FAILURE() << "Rollback failed: " << e.what();
    }
  }

  void resetTransactionThenConnection()
  {
    transaction_.reset();
    connection.reset();
  }
};

static void insertUser(pqxx::work &transaction, std::string_view name, std::string_view subject)
{
  const std::string query = "INSERT INTO users (name, subject) VALUES ($1, $2)";
  const pqxx::params parameters = {name, subject};
  transaction.exec(query, pqxx::params{name, subject});
}

static pqxx::result
retrieveUserCountByName(pqxx::transaction_base &transaction, std::string_view name)
{
  const std::string query = "SELECT COUNT(*) FROM users WHERE name = $1";
  const pqxx::params parameters = {name};
  return transaction.exec(query, parameters);
}

TEST_F(LibpqxxExplorationTests, ConnectToDB) { ASSERT_TRUE(true); }

TEST_F(LibpqxxExplorationTests, InsertUserIsRolledBackByTearDown)
{
  const std::string name = "TEST_NAME";
  const std::string subject = "TEST_SUBJECT";
  insertUser(transaction(), name, subject);

  pqxx::result r = retrieveUserCountByName(transaction(), name);
  ASSERT_EQ(r[0][0].as<long>(), 1);
}

TEST_F(LibpqxxExplorationTests, UncommittedTransactionIsNotVisible)
{
  const std::string name = "TEST_NAME";
  const std::string subject = "TEST_SUBJECT";
  insertUser(transaction(), name, subject);

  pqxx::connection alternateConnection = createConnection();
  pqxx::read_transaction readTransaction{alternateConnection};

  pqxx::result result = retrieveUserCountByName(readTransaction, name);
  ASSERT_EQ(result[0][0].as<long>(), 0);
}
