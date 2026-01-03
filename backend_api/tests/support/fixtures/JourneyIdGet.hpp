#pragma once

#include "cpr/api.h"
#include "cpr/response.h"
#include "cpr/timeout.h"
#include "support/fixtures/RouteFixture.hpp"
#include <cstdint>

class JourneyIdGet : public RouteFixture {
public:
  cpr::Response journeyIdGet(const std::string &token, int64_t id) {
    string bearer = std::format("Bearer {}", token);
    cpr::Header authHeader = {{"Authorization", bearer}};
    cpr::Timeout timeout = {TestConstants::TIMEOUT_MS};
    cpr::Url journeyUrl = {std::format("{}/{}/{}", url, "journey", id)};
    return cpr::Get(authHeader, journeyUrl, timeout);
  }
};
