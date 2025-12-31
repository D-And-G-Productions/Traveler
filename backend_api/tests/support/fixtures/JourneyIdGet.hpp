#pragma once

#include "cpr/api.h"
#include "cpr/response.h"
#include "cpr/timeout.h"
#include "support/fixtures/RouteFixture.hpp"
#include <cstdint>

class JourneyIdGet : public RouteFixture {
public:
  cpr::Response journeyGetId(int64_t id) {
    string validBearer = std::format("Bearer {}", TestConstants::TEST_TOKEN);
    return journeyGetId(validBearer, id);
  }

  cpr::Response journeyGetId(string &bearer, int64_t id) {
    cpr::Header authHeader = {{"Authorization", bearer}};
    cpr::Timeout timeout = {TestConstants::TIMEOUT_MS};
    cpr::Url journeyUrl = {std::format("{}/{}/{}", url, "journey", id)};
    return cpr::Get(authHeader, journeyUrl, timeout);
  }
};
