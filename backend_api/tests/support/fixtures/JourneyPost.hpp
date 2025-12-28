#pragma once

#include "api/dto/JourneyCreateRequest.hpp"
#include "api/json/JsonConversion.hpp"
#include "cpr/api.h"
#include "cpr/response.h"
#include "cpr/timeout.h"
#include "support/fixtures/RouteFixture.hpp"

class JourneyPost : public RouteFixture {
public:
  cpr::Response journeyPost(JourneyCreateRequest &journeyCreate) {
    string validBearer = std::format("Bearer {}", TestConstants::TEST_TOKEN);
    return journeyPost(validBearer, journeyCreate);
  }

  cpr::Response journeyPost(string &bearer, JourneyCreateRequest &journeyCreateRequest) {
    cpr::Header authHeader = {{"Authorization", bearer}};
    cpr::Timeout timeout = {TestConstants::TIMEOUT_MS};
    cpr::Url journeyUrl = {std::format("{}/{}", url, "journey")};
    cpr::Body body = api::json::toJson(journeyCreateRequest).dump();
    return cpr::Post(authHeader, journeyUrl, timeout, body);
  }
};
