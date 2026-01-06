#pragma once

#include "cpr/api.h"
#include "cpr/response.h"
#include "cpr/timeout.h"
#include "support/fixtures/RouteFixture.hpp"
#include <cstdint>

class JourneyIdDelete : public RouteFixture {
public:
  cpr::Response journeyIdDelete(const std::string &token, int64_t id) {
    string bearer = std::format("Bearer {}", token);
    cpr::Header authHeader = {{"Authorization", bearer}};
    cpr::Timeout timeout = {TestConstants::TIMEOUT_MS};
    cpr::Url journeyUrl = {std::format("{}/{}/{}", url, "journey", id)};
    return cpr::Delete(authHeader, journeyUrl, timeout);
  }
};
