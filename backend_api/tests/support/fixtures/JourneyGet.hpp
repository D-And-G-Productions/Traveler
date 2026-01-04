#pragma once

#include "cpr/api.h"
#include "cpr/response.h"
#include "cpr/timeout.h"
#include "support/fixtures/RouteFixture.hpp"
#include <crow/common.h>
#include <crow/http_request.h>
#include <crow/http_response.h>
#include <iostream>

class JourneyGet : public RouteFixture {
public:
  cpr::Response journeyGet(const string &token) {
    string bearer = std::format("Bearer {}", token);
    cpr::Header authHeader = {{"Authorization", bearer}};
    cpr::Timeout timeout = {TestConstants::TIMEOUT_MS};
    cpr::Url journeyUrl = {url + "/journey"};
    return cpr::Get(authHeader, journeyUrl, timeout);
  }
};
