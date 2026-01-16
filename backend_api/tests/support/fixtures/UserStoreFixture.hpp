#pragma once

#include "TestConstants.hpp"
#include "domain/User.hpp"
#include "persistence/DBPool.hpp"
#include "persistence/UserStore.hpp"
#include <gtest/gtest.h>
#include <memory>

class UserStoreFixture : public ::testing::Test
{
protected:
  std::unique_ptr<UserStore> userStore;
  User existingUser;

  static void SetUpTestSuite()
  {
    pool = std::make_unique<DBPool>(TestConstants::TEST_DATABASE_URL(), 1);
  }

  static void TearDownTestSuite() { pool.reset(); }

  void SetUp() override
  {
    lease = std::make_unique<DBPool::Lease>(pool->acquire());
    transaction = std::make_unique<pqxx::work>(lease->connection());
    userStore = std::make_unique<UserStore>(*transaction);
    existingUser = userStore->insertUser("SUBJECT");
  }

  void TearDown() override
  {
    userStore.reset();
    transaction->abort();
    transaction.reset();
    lease.reset();
  }

private:
  inline static std::unique_ptr<DBPool> pool{};
  std::unique_ptr<pqxx::work> transaction;
  std::unique_ptr<DBPool::Lease> lease;
};
