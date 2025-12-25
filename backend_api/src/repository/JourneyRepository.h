#pragma once
#include "domain/Journey.h"
#include "domain/JourneyRecord.h"
#include <cstdint>
#include <vector>

class JourneyRepository {
public:
  virtual ~JourneyRepository() = default;
  virtual JourneyRecord create(Journey journey) = 0;
  virtual JourneyRecord get(int64_t id) = 0;
  virtual JourneyRecord update(JourneyRecord journeyRecord) = 0;
  virtual std::vector<JourneyRecord> list() = 0;
};
