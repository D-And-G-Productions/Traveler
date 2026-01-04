#pragma once

#include "domain/JourneyCreate.hpp"
#include "domain/UserUpdate.hpp"
#include "support/TestServer.hpp"
#include "support/mocks/MockTokenVerifier.hpp"
#include <gtest/gtest.h>
#include <string>

struct TestUser {
  std::string token;
  std::string sub;
  User user;
};

class RouteFixture : public ::testing::Test {
public:
  std::string url;
  TestUser testUser;
  std::unique_ptr<TestServer> testServer;

  TestUser createTestUser(std::string name) {
    string token = std::to_string(userTokenCounter++);
    VerifiedToken verifiedToken = testServer->mockTokenVerifier->verifyAccessToken(token);
    string subject = verifiedToken.sub;
    User user = testServer->userRepo->insert(subject);
    UserUpdate userUpdate{.name = name};
    user = testServer->userRepo->update(user.id, userUpdate);
    return TestUser{.token = token, .sub = subject, .user = user};
  }

  Journey createJourney(JourneyCreate &journeyCreate) {
    return testServer->journeyRepo->insert(journeyCreate);
  }

protected:
  int userTokenCounter{0};

  void SetUp() override {
    testServer = std::make_unique<TestServer>();
    url = std::format("http://{}:{}", testServer->ADDRESS, testServer->PORT);
    testServer->initialise();
    testServer->start();
    testUser = createTestUser("TEST_USER");
  }

  void TearDown() override {
    testServer->stop();
    testServer.reset();
  }
};
