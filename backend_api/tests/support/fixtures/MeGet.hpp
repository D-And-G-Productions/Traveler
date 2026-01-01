#pragma once

#include "cpr/api.h"
#include "cpr/response.h"
#include "cpr/timeout.h"
#include "support/fixtures/RouteFixture.hpp"

struct MeGet : public RouteFixture {
  cpr::Response meGet() {
    string validBearer = std::format("Bearer {}", TestConstants::TEST_TOKEN);
    cpr::Header authHeader = {{"Authorization", validBearer}};
    cpr::Timeout timeout = {TestConstants::TIMEOUT_MS};
    cpr::Url journeyUrl = {std::format("{}/{}", url, "me")};
    return cpr::Get(authHeader, journeyUrl, timeout);
  }
};
