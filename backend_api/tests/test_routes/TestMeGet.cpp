#include "api/dto/UserReponse.hpp"
#include "support/fixtures/MeGet.hpp"
#include "support/mocks/MockTokenVerifier.hpp"
#include <cpr/response.h>
#include <cpr/status_codes.h>
#include <crow/common.h>
#include <crow/json.h>
#include <gtest/gtest.h>
#include <string>

void addUserWithName(std::unique_ptr<TestServer> &server, const std::string name) {
  const User user = server->userRepository->insert(MockTokenVerifier::TESTING_SUBJECT);
  const UserUpdate userUpdate{.name = name};
  server->userRepository->update(user.id, userUpdate);
}

TEST_F(MeGet, ReturnsUserData) {
  const std::string TEST_NAME = "TEST_NAME";
  addUserWithName(server, TEST_NAME);

  const cpr::Response response = meGet();
  const crow::json::rvalue parsed = crow::json::load(response.text);
  ASSERT_TRUE(parsed);

  const UserResponse userResponse = api::json::fromJson(parsed);
  EXPECT_EQ(userResponse.name, TEST_NAME) << response.text << "\n";
}

TEST_F(MeGet, ReturnsOk) {
  server->userRepository->insert(MockTokenVerifier::TESTING_SUBJECT);
  cpr::Response response = meGet();
  ASSERT_EQ(response.status_code, cpr::status::HTTP_OK) << response.text << "\n";
}
