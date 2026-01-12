#pragma once

#include "cpr/api.h"
#include "fixtures/RouteFixture.hpp"

class HealthRoutes : public RouteFixture
{
public:
  cpr::Response healthGet(const std::string &token)
  {
    cpr::Header authHeader = buildAuthorisationHeader(token);
    cpr::Timeout timeout = {TestConstants::TIMEOUT_MS};
    cpr::Url url = {std::format("{}/{}", serverUrl(), "health")};
    return cpr::Get(authHeader, url, timeout);
  }
};
