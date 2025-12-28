#pragma once

#include "api/dto/JourneyCreateRequest.hpp"
#include "cpr/api.h"
#include "cpr/body.h"
#include "cpr/response.h"
#include "cpr/timeout.h"
#include "support/fixtures/RouteFixture.hpp"
#include <cstdint>

class JourneyIdPut : public RouteFixture {
public:
  cpr::Response journeyIdPut(const int64_t id, const JourneyCreateRequest &jcRequest) {
    string validBearer = std::format("Bearer {}", TestConstants::TEST_TOKEN);
    return journeyIdPut(validBearer, id, jcRequest);
  }

  cpr::Response
  journeyIdPut(const string &bearer, const int64_t id, const JourneyCreateRequest &jcRequest) {
    cpr::Header authHeader = {{"Authorization", bearer}};
    cpr::Timeout timeout = {TestConstants::TIMEOUT_MS};
    cpr::Url journeyUrl = {std::format("{}/{}/{}", url, "journey", id)};
    cpr::Body body = api::json::toJson(jcRequest).dump();
    return cpr::Put(authHeader, journeyUrl, timeout, body);
  }
};
