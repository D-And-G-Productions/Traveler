#include "authentication/TokenVerifier.hpp"
#include "http/middleware/AuthMiddleware.hpp"
#include "support/fixtures/AuthMiddlewareFixture.hpp"
#include "support/mocks/MockTokenVerifier.hpp"
#include <crow/http_request.h>
#include <gtest/gtest.h>
#include <memory>

TEST_F(AuthMiddlewareFixture, MissingAuthHeaderReturns401AndMessage) {
  acceptAllTokens();
  crow::request request{};
  crow::response response{};
  AuthMiddleware::context context{};
  authMiddleware->before_handle(request, response, context);

  EXPECT_EQ(response.code, 401);
  EXPECT_EQ(response.body, "Missing bearer token");
  EXPECT_FALSE(context.authorized);
}

TEST_F(AuthMiddlewareFixture, NonBearerAuthReturns401AndMessage) {
  acceptAllTokens();
  string authHeader = "DOES_NOT_SAY_BEARER";
  crow::request request = createRequest(authHeader);
  crow::response response{};
  AuthMiddleware::context context{};
  authMiddleware->before_handle(request, response, context);

  EXPECT_EQ(response.code, 401);
  EXPECT_EQ(response.body, "Missing bearer token");
  EXPECT_FALSE(context.authorized);
}

TEST_F(AuthMiddlewareFixture, InvalidTokenReturns401AndDoesNotAuthorize) {
  rejectAllTokens();
  crow::request request = createRequest("Bearer INVALID_AUTH_TOKEN");
  crow::response response{};
  AuthMiddleware::context context{};
  authMiddleware->before_handle(request, response, context);

  EXPECT_EQ(response.code, 401);
  EXPECT_FALSE(context.authorized);
}

TEST_F(AuthMiddlewareFixture, ValidTokenSetsContextAndLeavesResponseUntouched) {
  acceptAllTokens();
  std::string TOKEN_AND_SUB = "FOO";
  crow::request request = createRequest(std::format("Bearer {}", TOKEN_AND_SUB));
  crow::response response{};
  AuthMiddleware::context context{};
  authMiddleware->before_handle(request, response, context);

  EXPECT_TRUE(context.authorized);
  EXPECT_EQ(response.code, 200);
  EXPECT_TRUE(response.body.empty());
  EXPECT_EQ(context.verifiedToken.sub, TOKEN_AND_SUB);
}
