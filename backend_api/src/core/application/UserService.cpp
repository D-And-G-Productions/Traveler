#include "UserService.hpp"
#include "persistence/UserStore.hpp"

User UserService::getUser(const std::string subject) const
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
