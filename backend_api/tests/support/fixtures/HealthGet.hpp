#pragma once

#include "cpr/api.h"
#include "cpr/response.h"
#include "cpr/timeout.h"
#include "support/fixtures/RouteFixture.hpp"

class HealthGet : public RouteFixture {
public:
  cpr::Response healthGet(const std::string &token) {
    string bearer = std::format("Bearer {}", token);
    cpr::Header authHeader = {{"Authorization", bearer}};
    cpr::Timeout timeout = {TestConstants::TIMEOUT_MS};
    cpr::Url journeyUrl = {std::format("{}/{}", url, "journey")};
    return cpr::Get(authHeader, journeyUrl, timeout);
  }
};
