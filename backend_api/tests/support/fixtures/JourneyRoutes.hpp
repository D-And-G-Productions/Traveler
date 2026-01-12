#pragma once

#include "api/dto/JourneyCreateRequestContract.hpp"
#include "api/dto/JourneyUpdateRequest.hpp"
#include "fixtures/RouteFixture.hpp"
#include <cstdint>

class JourneyRoutes : public RouteFixture
{
public:
  cpr::Response journeysGet(const std::string &token);

  cpr::Response journeyIdGet(const std::string &token, const int64_t id);

  using CreateContract = JourneyCreateRequestContract;
  cpr::Response journeyPost(const std::string &token, const CreateContract &jcr);

  cpr::Response journeyIdDelete(const std::string &token, const int64_t id);

  using UpdateContract = JourneyUpdateRequestContract;
  cpr::Response journeyIdPut(const std::string &token, const int64_t id, const UpdateContract &j);

  Journey createJourneyForUser(const int64_t userId);

  Journey getJourneyFromDB(const int64_t journeyId);
};
