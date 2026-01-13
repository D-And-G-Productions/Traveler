#pragma once

#include "TestConstants.hpp"
#include "TestUser.hpp"
#include "application/UserService.hpp"
#include "persistence/DBPool.hpp"
#include <cstdlib>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>

namespace TestCommon
{
static void resetDatabase()
{
  pqxx::connection conn{TestConstants::TEST_DATABASE_URL()};
  pqxx::work tx{conn};
  std::string query = R"SQL(
      TRUNCATE locations RESTART IDENTITY CASCADE;
      TRUNCATE users RESTART IDENTITY CASCADE;
    )SQL";
  tx.exec(query);
  tx.commit();
}

static User createUserFromSub(const std::string_view subject)
{
  std::shared_ptr<DBPool> pool = std::make_shared<DBPool>(TestConstants::TEST_DATABASE_URL(), 1);
  UserService service{pool};
  User createdUser = service.insertUser(subject);
  return createdUser;
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
