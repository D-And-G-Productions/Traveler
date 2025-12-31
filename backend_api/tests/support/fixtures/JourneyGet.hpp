#pragma once

#include "cpr/api.h"
#include "cpr/response.h"
#include "cpr/timeout.h"
#include "support/fixtures/RouteFixture.hpp"

class JourneyGet : public RouteFixture {
public:
  cpr::Response journeyGet() {
    string validBearer = std::format("Bearer {}", TestConstants::TEST_TOKEN);
    return journeyGet(validBearer);
  }

  cpr::Response journeyGet(string bearer) {
    cpr::Header authHeader = {{"Authorization", bearer}};
    cpr::Timeout timeout = {TestConstants::TIMEOUT_MS};
    cpr::Url journeyUrl = {url + "/journey"};
    return cpr::Get(authHeader, journeyUrl, timeout);
  }
};
