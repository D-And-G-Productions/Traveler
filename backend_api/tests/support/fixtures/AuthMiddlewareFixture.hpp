#pragma once

#include "TestCommon.hpp"
#include "TestConstants.hpp"
#include "http/middleware/AuthMiddleware.hpp"
#include "mocks/MockTokenVerifier.hpp"
#include <crow/http_request.h>
#include <gtest/gtest.h>
#include <memory>

class AuthMiddlewareFixture : public ::testing::Test
{
public:
  std::unique_ptr<AuthMiddleware> authMiddleware;

protected:
  std::shared_ptr<DBPool> pool;
  std::shared_ptr<UserService> authService;
  std::shared_ptr<TokenVerifier> tokenVerifier;

  void SetUp() override
  {
    pool = std::make_shared<DBPool>(TestConstants::TEST_DATABASE_URL(), 4);
    authService = std::make_shared<UserService>(pool);
    tokenVerifier = std::make_shared<MockTokenVerifier>(pool);

    authMiddleware = std::make_unique<AuthMiddleware>();
    authMiddleware->setDeps(tokenVerifier, authService);
  }

  void TearDown() override { TestCommon::resetDatabase(); }
};
