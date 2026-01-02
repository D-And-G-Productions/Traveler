#pragma once

#include "http/middleware/AuthMiddleware.hpp"
#include "support/mocks/MockTokenVerifier.hpp"
#include "support/mocks/MockUserRepository.hpp"
#include <crow/http_request.h>
#include <gtest/gtest.h>
#include <memory>

using std::format;
using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::unique_ptr;

class AuthMiddlewareFixture : public ::testing::Test {
public:
  shared_ptr<MockTokenVerifier> mockTokenVerifier;
  shared_ptr<UserRepository> userRepository;
  unique_ptr<AuthMiddleware> authMiddleware;

  crow::request createRequest(const std::string &bearer) {
    crow::request request{};
    request.headers.emplace("Authorization", bearer);
    return request;
  }

  void acceptAllTokens() { mockTokenVerifier->acceptAll(); }

  void rejectAllTokens() { mockTokenVerifier->rejectAll(); }

private:
  void SetUp() override {
    authMiddleware = make_unique<AuthMiddleware>();
    mockTokenVerifier = make_shared<MockTokenVerifier>();
    authMiddleware->setVerifier(mockTokenVerifier);
    userRepository = make_shared<MockUserRepository>();
    authMiddleware->setUserRepository(userRepository);
  }

  void TearDown() override {
    authMiddleware.reset();
    mockTokenVerifier.reset();
  }
};
