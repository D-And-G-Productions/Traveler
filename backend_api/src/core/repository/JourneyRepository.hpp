#pragma once

#include "domain/Journey.hpp"
#include "domain/JourneyCreate.hpp"
#include <cstdint>
#include <stdexcept>
#include <vector>

class JourneyNotFoundError : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

class JourneyRepository {
public:
  virtual ~JourneyRepository() = default;
  virtual Journey insert(const JourneyCreate &journey) = 0;
  virtual Journey selectById(int64_t id) = 0;
  virtual std::vector<Journey> selectByUserId(int64_t userId) = 0;
  virtual Journey update(const int64_t id, const JourneyCreate &updatedJourney) = 0;
  virtual std::vector<Journey> list() = 0;
};
