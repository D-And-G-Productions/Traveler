#pragma once

#include "cpr/api.h"
#include "cpr/response.h"
#include "cpr/timeout.h"
#include "support/fixtures/RouteFixture.hpp"

struct MeGet : public RouteFixture {
  cpr::Response meGet(const std::string &token) {
    string bearer = std::format("Bearer {}", token);
    cpr::Header authHeader = {{"Authorization", bearer}};
    cpr::Timeout timeout = {TestConstants::TIMEOUT_MS};
    cpr::Url journeyUrl = {std::format("{}/{}", url, "me")};
    return cpr::Get(authHeader, journeyUrl, timeout);
  }

  void expectMatchingUserData(const User &user, const UserResponse &ur) {
    EXPECT_EQ(ur.name, user.name);
    EXPECT_EQ(ur.telephone, user.telephone);
  }

  const UserResponse parseToUserResponse(const string responseBody) {
    const rvalue userJson = crow::json::load(responseBody);
    const UserResponse userResponse = api::json::fromJson(userJson);
    return userResponse;
  }
};
