#include "TestCommon.hpp"
#include "cpr/status_codes.h"
#include "fixtures/AuthMiddlewareFixture.hpp"
#include "http/middleware/AuthMiddleware.hpp"
#include "mocks/MockTokenVerifier.hpp"
#include <crow/http_request.h>
#include <gtest/gtest.h>

namespace
{
crow::request createRequest(const std::string &bearer)
{
  crow::request request{};
  request.headers.emplace("Authorization", bearer);
  return request;
}
} // namespace

TEST_F(AuthMiddlewareFixture, ValidTokenIsAuthorized)
{
  const std::string USER_TOKEN = "VALID_USER_TOKEN";
  TestCommon::createTestUser(USER_TOKEN);
  std::string bearer = std::format("Bearer {}", USER_TOKEN);
  crow::request request = createRequest(bearer);
  crow::response response{};
  AuthMiddleware::context context{};
  authMiddleware->before_handle(request, response, context);

  ASSERT_EQ(response.code, cpr::status::HTTP_OK);
}

TEST_F(AuthMiddlewareFixture, MissingAuthHeaderReturnsUnauthorized)
{
  crow::request request{};
  crow::response response{};
  AuthMiddleware::context context{};
  authMiddleware->before_handle(request, response, context);

  ASSERT_EQ(response.code, cpr::status::HTTP_UNAUTHORIZED);
}

TEST_F(AuthMiddlewareFixture, NonBearerAuthReturnsUnauthorized)
{
  string authHeader = "DOES_NOT_SAY_BEARER";
  crow::request request = createRequest(authHeader);
  crow::response response{};
  AuthMiddleware::context context{};
  authMiddleware->before_handle(request, response, context);

  ASSERT_EQ(response.code, cpr::status::HTTP_UNAUTHORIZED);
}

TEST_F(AuthMiddlewareFixture, InvalidTokenReturnsUnauthorized)
{
  crow::request request = createRequest("Bearer INVALID_AUTH_TOKEN");
  crow::response response{};
  AuthMiddleware::context context{};
  authMiddleware->before_handle(request, response, context);

  ASSERT_EQ(response.code, cpr::status::HTTP_UNAUTHORIZED);
}
