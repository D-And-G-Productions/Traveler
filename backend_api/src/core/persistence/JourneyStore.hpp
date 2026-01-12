#pragma once

#include "Store.hpp"
#include "domain/Journey.hpp"
#include "domain/JourneyCreate.hpp"
#include "domain/JourneyUpdate.hpp"
#include <cstdint>
#include <pqxx/internal/statement_parameters.hxx>
#include <pqxx/pqxx>
#include <vector>

class JourneyStore : public Store
{
public:
  using Store::Store;
  std::vector<Journey> selectJourneysByUserId(const int64_t userId);
  Journey insertJourney(const JourneyCreate &jc);
  Journey selectJourney(const int64_t journeyId);
  Journey selectJourney(const int64_t journeyId, const int64_t userId);
  void deleteJourney(const int64_t journeyId, const int64_t userId);
  Journey updateJourney(const int64_t journeyId, const int64_t userId, const JourneyUpdate &update);
};
