#pragma once

#include "domain/Journey.hpp"
#include "domain/JourneyCreate.hpp"
#include "domain/JourneyUpdate.hpp"
#include "persistence/DBPool.hpp"
#include <cstdint>
#include <vector>

class JourneyService
{
public:
  JourneyService(std::shared_ptr<DBPool> p) : pool(std::move(p)) {}

  Journey getJourney(const int64_t journeyId, const int64_t userId) const;
  std::vector<Journey> getJourneys(const int64_t userId) const;
  Journey addJourney(const JourneyCreate &jc);
  Journey updateJourney(const int64_t journeyId, const int64_t userId, const JourneyUpdate &ju);
  void deleteJourney(const int64_t journeyId, const int64_t userId);

private:
  std::shared_ptr<DBPool> pool;
};
