#include "api/dto/UserReponse.hpp"
#include "support/fixtures/MeGet.hpp"
#include "support/fixtures/RouteFixture.hpp"
#include <cpr/response.h>
#include <cpr/status_codes.h>
#include <crow/common.h>
#include <crow/json.h>
#include <gtest/gtest.h>
#include <string>

using crow::json::rvalue;
using std::string;

TEST_F(MeGet, ReturnsOk) {
  cpr::Response response = meGet(testUser.token);
  ASSERT_EQ(response.status_code, cpr::status::HTTP_OK) << response.text << "\n";
}

TEST_F(MeGet, ReturnsUserData) {
  const cpr::Response response = meGet(testUser.token);
  const rvalue userJson = crow::json::load(response.text);

  ASSERT_TRUE(userJson);
  EXPECT_NO_THROW(const UserResponse userResponse = api::json::fromJson(userJson));
}

TEST_F(MeGet, UserDataMatches) {
  const cpr::Response response = meGet(testUser.token);
  const UserResponse userResponse = parseToUserResponse(response.text);
  expectMatchingUserData(testUser.user, userResponse);
}

TEST_F(MeGet, UsersGetTheirOwnData) {
  TestUser differentTestUser = createTestUser("different USER NAME");
  const cpr::Response testUserHttpResponse = meGet(testUser.token);
  const cpr::Response differentUserHttpResponse = meGet(differentTestUser.token);
  UserResponse testUserResponse = parseToUserResponse(testUserHttpResponse.text);
  UserResponse differentUserResponse = parseToUserResponse(differentUserHttpResponse.text);
  expectMatchingUserData(testUser.user, testUserResponse);
  expectMatchingUserData(differentTestUser.user, differentUserResponse);
}
