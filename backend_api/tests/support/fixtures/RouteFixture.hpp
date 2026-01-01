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
  std::unique_ptr<TestServer> server;

  TestUser createTestUser(std::string name) {
    string token = std::to_string(userTokenCounter++);
    VerifiedToken verifiedToken = server->mockTokenVerifier->verifyAccessToken(token);
    string subject = verifiedToken.sub;
    User user = server->userRepo->insert(subject);
    UserUpdate userUpdate{.name = name};
    server->userRepo->update(user.id, userUpdate);
    return TestUser{.token = token, .sub = subject, .user = user};
  }

  Journey createJourney(JourneyCreate &journeyCreate) {
    return server->journeyRepo->insert(journeyCreate);
  }

protected:
  int userTokenCounter{0};

  void SetUp() override {
    server = std::make_unique<TestServer>();
    url = std::format("http://{}:{}", server->ADDRESS, server->PORT);
    server->initialise();
    server->start();
    testUser = createTestUser("TEST_USER");
  }

  void TearDown() override {
    server->stop();
    server.reset();
  }
};
