#include "api/json/JsonConversion.hpp"
#include "cpr/status_codes.h"
#include "fixtures/MeRoutes.hpp"

TEST_F(MeRoutes, ReturnsOk)
{
  const cpr::Response response = meGet(testUser.token);

  ASSERT_EQ(response.status_code, cpr::status::HTTP_OK) << "Response body: " << response.text;
}

TEST_F(MeRoutes, ReturnsValidJsonUser)
{
  const cpr::Response response = meGet(testUser.token);
  const crow::json::rvalue json = crow::json::load(response.text);

  ASSERT_TRUE(json) << "Invalid JSON: " << response.text;
  EXPECT_NO_THROW({ JsonConversion::fromJson<UserResponseContract>(json); });
}

TEST_F(MeRoutes, ReturnedUserMatchesAuthenticatedUser)
{
  const cpr::Response response = meGet(testUser.token);
  const crow::json::rvalue json = crow::json::load(response.text);
  const UserResponseContract urc = JsonConversion::fromJson<UserResponseContract>(json);

  ASSERT_TRUE(isSameUser(testUser.user, urc));
}

TEST_F(MeRoutes, UsersOnlySeeTheirOwnData)
{
  const cpr::Response response = meGet(testUser.token);
  const crow::json::rvalue json = crow::json::load(response.text);
  const UserResponseContract urc = JsonConversion::fromJson<UserResponseContract>(json);

  TestUser otherUser = createTestUser();
  ASSERT_FALSE(isSameUser(otherUser.user, urc));
}
