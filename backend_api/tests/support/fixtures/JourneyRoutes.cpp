#include "JourneyRoutes.hpp"
#include "api/dto/JourneyCreateRequestContract.hpp"
#include "api/dto/JourneyUpdateRequest.hpp"
#include "api/json/JsonConversion.hpp"
#include "cpr/api.h"
#include "cpr/body.h"
#include "domain/JourneyCreate.hpp"
#include "domain/Mode.hpp"

#include <cstdint>

cpr::Response JourneyRoutes::journeysGet(const std::string &token)
{
  cpr::Header authHeader = buildAuthorisationHeader(token);
  cpr::Timeout timeout = {TestConstants::TIMEOUT_MS};
  cpr::Url url = {std::format("{}/{}", serverUrl(), "journey")};
  return cpr::Get(authHeader, url, timeout);
}

cpr::Response JourneyRoutes::journeyIdGet(const std::string &token, const int64_t id)
{
  cpr::Header authHeader = buildAuthorisationHeader(token);
  cpr::Timeout timeout = {TestConstants::TIMEOUT_MS};
  cpr::Url url = {std::format("{}/{}/{}", serverUrl(), "journey", id)};
  return cpr::Get(authHeader, url, timeout);
}

using CreateContract = JourneyCreateRequestContract;

cpr::Response JourneyRoutes::journeyPost(const std::string &token, const CreateContract &jcr)
{
  cpr::Header authHeader = buildAuthorisationHeader(token);
  cpr::Timeout timeout = {TestConstants::TIMEOUT_MS};
  cpr::Url url = {std::format("{}/{}", serverUrl(), "journey")};
  cpr::Body body = JsonConversion::toJson<JourneyCreateRequestContract>(jcr).dump();
  return cpr::Post(authHeader, url, timeout, body);
}

cpr::Response JourneyRoutes::journeyIdDelete(const std::string &token, const int64_t id)
{
  cpr::Header authHeader = buildAuthorisationHeader(token);
  cpr::Timeout timeout = {TestConstants::TIMEOUT_MS};
  cpr::Url url = {std::format("{}/{}/{}", serverUrl(), "journey", id)};
  return cpr::Delete(authHeader, url, timeout);
}

using UpdateContract = JourneyUpdateRequestContract;

cpr::Response
JourneyRoutes::journeyIdPut(const std::string &token, const int64_t id, const UpdateContract &j)
{
  cpr::Header authHeader = buildAuthorisationHeader(token);
  cpr::Timeout timeout = {TestConstants::TIMEOUT_MS};
  cpr::Url url = {std::format("{}/{}/{}", serverUrl(), "journey", id)};
  cpr::Body body = JsonConversion::toJson(j).dump();
  return cpr::Put(authHeader, url, timeout, body);
}

Journey JourneyRoutes::createJourneyForUser(const int64_t userId)
{
  JourneyCreate jc{
      .userId = userId,
      .arrivalTime = "2026-01-09T12:00:00+00:00",
      .mode = Mode::TRANSIT,
  };
  return server->createJourney(jc);
}

Journey JourneyRoutes::getJourneyFromDB(const int64_t journeyId)
{
  return server->journeyFromDB(journeyId);
}
