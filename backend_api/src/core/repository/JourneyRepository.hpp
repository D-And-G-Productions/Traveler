#pragma once

#include "domain/Journey.hpp"
#include "domain/JourneyCreate.hpp"
#include <cstdint>
#include <vector>

class JourneyRepository {
public:
  virtual ~JourneyRepository() = default;
  virtual Journey insert(const JourneyCreate &journey) = 0;
  virtual Journey selectById(int64_t id) = 0;
  virtual std::vector<Journey> selectByUserId(int64_t userId) = 0;
  virtual Journey update(int64_t id, JourneyCreate &updatedJourney) = 0;
  virtual void del(int64_t id) = 0;
  virtual std::vector<Journey> list() = 0;
};
