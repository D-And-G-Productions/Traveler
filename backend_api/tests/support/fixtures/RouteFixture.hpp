#pragma once

#include "TestCommon.hpp"
#include "TestConstants.hpp"
#include "TestServer.hpp"
#include "TestUser.hpp"
#include <cpr/response.h>
#include <format>
#include <gtest/gtest.h>
#include <pqxx/pqxx>
#include <string>
#include <string_view>

class RouteFixture : public ::testing::Test
{
protected:
  TestUser testUser;

  static void SetUpTestSuite()
  {
    serverUrl_ =
        std::format("http://{}:{}", TestConstants::LOCAL_ADDRESS, TestConstants::TEST_PORT);

    server = std::make_unique<TestServer>(TestConstants::TEST_DATABASE_URL());
    server->initialise();
    server->start();
  }

  static void TearDownTestSuite()
  {
    if (server)
    {
      server->stop();
      server.reset();
    }
  }

  void SetUp() override { testUser = createTestUser(); }

  void TearDown() override { resetDatabase(); }

  static void resetDatabase() { TestCommon::resetDatabase(); }

  static TestUser createTestUser()
  {
    const std::string token = std::to_string(tokenCount++);
    return TestCommon::createTestUser(token);
  }

  cpr::Header buildAuthorisationHeader(const std::string_view token)
  {
    std::string bearer = std::format("Bearer {}", token);
    return {{"Authorization", bearer}};
  }

  static const std::string &serverUrl() { return serverUrl_; }

protected:
  inline static std::unique_ptr<TestServer> server{};
  inline static std::string serverUrl_{};
  inline static int tokenCount{0};
};
