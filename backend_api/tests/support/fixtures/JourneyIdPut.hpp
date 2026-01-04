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
  cpr::Response
  journeyIdPut(const string &token, const int64_t id, const JourneyCreateRequest &jcRequest) {
    string bearer = std::format("Bearer {}", TestConstants::TEST_TOKEN);
    cpr::Header authHeader = {{"Authorization", bearer}};
    cpr::Timeout timeout = {TestConstants::TIMEOUT_MS};
    cpr::Url journeyUrl = {std::format("{}/{}/{}", url, "journey", id)};
    cpr::Body body = api::json::toJson(jcRequest).dump();
    return cpr::Put(authHeader, journeyUrl, timeout, body);
  }
};
