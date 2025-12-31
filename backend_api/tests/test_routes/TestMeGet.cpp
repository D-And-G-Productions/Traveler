#include "api/dto/UserReponse.hpp"
#include "support/fixtures/MeGet.hpp"
#include "support/mocks/MockTokenVerifier.hpp"
#include <cpr/response.h>
#include <cpr/status_codes.h>
#include <crow/common.h>
#include <crow/json.h>
#include <gtest/gtest.h>
#include <string>

using crow::json::rvalue;
using std::string;

void addUserWithName(
    std::unique_ptr<TestServer> &server, const std::string name, const std::string subject
) {
  const User user = server->userRepo->insert(subject);
  server->userRepo->update(user.id, {.name = name});
}

TEST_F(MeGet, ReturnsUserData) {
  const string TEST_NAME = "FOO";
  const string TOKEN_AND_SUB = "BAR";
  addUserWithName(server, TEST_NAME, TOKEN_AND_SUB);
  const cpr::Response response = meGet(TOKEN_AND_SUB);
  const rvalue parsed = crow::json::load(response.text);

  ASSERT_TRUE(parsed);
  const UserResponse userResponse = api::json::fromJson(parsed);
  EXPECT_EQ(userResponse.name, TEST_NAME) << response.text << "\n";
}

TEST_F(MeGet, ReturnsOk) {
  const string TOKEN_AND_SUB = "BAR";
  server->userRepo->insert(TOKEN_AND_SUB);
  cpr::Response response = meGet(TOKEN_AND_SUB);
  ASSERT_EQ(response.status_code, cpr::status::HTTP_OK) << response.text << "\n";
}
