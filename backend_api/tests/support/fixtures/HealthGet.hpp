#pragma once

#include "cpr/api.h"
#include "cpr/response.h"
#include "cpr/timeout.h"
#include "support/fixtures/RouteFixture.hpp"

class HealthGet : public RouteFixture {
public:
  cpr::Response healthGet() {
    string validBearer = std::format("Bearer {}", TestConstants::TEST_TOKEN);
    cpr::Header authHeader = {{"Authorization", validBearer}};
    cpr::Timeout timeout = {TestConstants::TIMEOUT_MS};
    cpr::Url journeyUrl = {std::format("{}/{}", url, "journey")};
    return cpr::Get(authHeader, journeyUrl, timeout);
  }
};
