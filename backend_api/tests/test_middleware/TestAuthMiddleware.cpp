#include "authentication/TokenVerifier.hpp"
#include "http/middleware/AuthMiddleware.hpp"
#include "support/mocks/MockTokenVerifier.hpp"
#include <crow/http_request.h>
#include <gtest/gtest.h>
#include <memory>
#include <utility>

using std::format;
using std::make_shared;
using std::make_unique;
using std::shared_ptr;

class AuthMiddlewareFixture : public ::testing::Test {
public:
  std::shared_ptr<TokenVerifier> tokenVerifier;
  std::unique_ptr<AuthMiddleware> authMiddleware;

  crow::request makeRequestWithAuthHeader(const std::string &authHeader) {
    crow::request r{};
    r.headers.emplace("Authorization", authHeader);
    return r;
  }

private:
  void SetUp() override {
    tokenVerifier = make_shared<MockTokenVerifier>();
    authMiddleware = make_unique<AuthMiddleware>();
    authMiddleware->setVerifier(std::move(tokenVerifier));
  }

  void TearDown() override {
    authMiddleware.reset();
    tokenVerifier.reset();
  }
};

TEST_F(AuthMiddlewareFixture, MissingAuthHeaderReturns401AndMessage) {
  crow::request request{};
  crow::response response{};
  AuthMiddleware::context context{};
  authMiddleware->before_handle(request, response, context);

  EXPECT_EQ(response.code, 401);
  EXPECT_EQ(response.body, "Missing bearer token");
  EXPECT_FALSE(context.authorized);
}

TEST_F(AuthMiddlewareFixture, NonBearerAuthReturns401AndMessage) {
  string authHeader = "DOES_NOT_SAY_BEARER";
  crow::request request = makeRequestWithAuthHeader(authHeader);
  crow::response response{};
  AuthMiddleware::context context{};
  authMiddleware->before_handle(request, response, context);

  EXPECT_EQ(response.code, 401);
  EXPECT_EQ(response.body, "Missing bearer token");
  EXPECT_FALSE(context.authorized);
}

TEST_F(AuthMiddlewareFixture, InvalidTokenReturns401AndDoesNotAuthorize) {
  string authHeader = "Bearer INVALID_AUTH_TOKEN";
  crow::request request = makeRequestWithAuthHeader(authHeader);
  crow::response response{};
  AuthMiddleware::context context{};
  authMiddleware->before_handle(request, response, context);

  EXPECT_EQ(response.code, 401);
  EXPECT_FALSE(context.authorized);
}

TEST_F(AuthMiddlewareFixture, ValidTokenSetsContextAndLeavesResponseUntouched) {
  string authHeader = format("Bearer {}", MockTokenVerifier::TESTING_TOKEN);
  crow::request request = makeRequestWithAuthHeader(authHeader);
  crow::response response{};
  AuthMiddleware::context context{};
  authMiddleware->before_handle(request, response, context);

  EXPECT_TRUE(context.authorized);
  EXPECT_EQ(response.code, 200);
  EXPECT_TRUE(response.body.empty());
  EXPECT_EQ(context.token.sub, MockTokenVerifier::TESTING_SUBJECT);
  EXPECT_EQ(context.token.issuer, MockTokenVerifier::TESTING_ISSUER);
}
