#pragma once

#include "TestUser.hpp"
#include "persistence/DBPool.hpp"
#include "persistence/UserStore.hpp"
#include <cstdlib>
#include <stdexcept>
#include <string>

namespace TestCommon
{
static std::string requireEnv(const char *key)
{
  const char *variable = std::getenv(key);
  if (variable && *variable)
  {
    return std::string{variable};
  }
  throw std::runtime_error(std::string{"Missing env variable: "} + key);
}

static std::string testDBUrl() { return requireEnv("DATABASE_URL"); }

static void resetDatabase()
{
  pqxx::connection conn{testDBUrl()};
  pqxx::work tx{conn};
  std::string query = R"SQL(
      TRUNCATE locations RESTART IDENTITY CASCADE;
      TRUNCATE users RESTART IDENTITY CASCADE;
    )SQL";
  tx.exec(query);
  tx.commit();
}

static User createUserFromSub(const std::string &subject)
{
  pqxx::connection conn{testDBUrl()};
  pqxx::work transaction{conn};
  UserStore userStore{transaction};
  User user = userStore.insertUserBySubject(subject);
  transaction.commit();
  return user;
}

static TestUser createTestUser(const std::string token)
{
  User user = createUserFromSub(token);
  return {
      .token = token,
      .user = user,
  };
}
} // namespace TestCommon
