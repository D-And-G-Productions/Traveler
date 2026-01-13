#include "UserService.hpp"
#include "persistence/UserStore.hpp"
#include <string_view>

User UserService::getUser(const std::string_view subject) const
{
  DBPool::Lease lease = pool->acquire();
  pqxx::read_transaction transaction{lease.connection()};
  UserStore userStore{transaction};
  return userStore.selectUser(subject);
}

User UserService::getUser(const int64_t userId) const
{
  DBPool::Lease lease = pool->acquire();
  pqxx::read_transaction transaction{lease.connection()};
  UserStore userStore{transaction};
  return userStore.selectUser(userId);
}

User UserService::insertUser(const std::string_view subject)
{
  DBPool::Lease lease = pool->acquire();
  pqxx::work transaction{lease.connection()};
  UserStore userStore{transaction};
  transaction.commit();
  return userStore.insertUserBySubject(subject);
};
