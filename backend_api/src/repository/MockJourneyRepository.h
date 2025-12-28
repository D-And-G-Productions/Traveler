#pragma once
#include "JourneyRepository.h"
#include "domain/Journey.h"
#include <atomic>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <ostream>
#include <unordered_map>
#include <utility>
#include <vector>

class MockJourneyRepository final : public JourneyRepository {
public:
  JourneyRecord create(Journey &journey) override;

  JourneyRecord get(int64_t id) override;

  JourneyRecord update(JourneyRecord &updatedJourneyRecord) override;

  std::vector<JourneyRecord> list() override;

private:
  mutable std::mutex journeysMutex;

  std::unordered_map<int, JourneyRecord> journeys;

  std::atomic<int64_t> index{0};
};

inline JourneyRecord MockJourneyRepository::create(Journey &journey) {
  int64_t id = index.fetch_add(1, std::memory_order_relaxed);
  JourneyRecord journeyRecord{id, journey};
  std::lock_guard<std::mutex> lock(journeysMutex);
  auto [it, ok] = journeys.insert(std::make_pair(id, journeyRecord));
  return it->second;
}

inline JourneyRecord MockJourneyRepository::get(int64_t id) {
  std::lock_guard<std::mutex> lock(journeysMutex);
  auto iterator = journeys.find(id);
  if (iterator == journeys.end()) {
    throw std::invalid_argument("Id does not exist");
  }
  return iterator->second;
}

inline JourneyRecord MockJourneyRepository::update(JourneyRecord &updatedJourneyRecord) {
  auto iterator = journeys.find(updatedJourneyRecord.id);
  if (iterator == journeys.end()) {
    throw std::invalid_argument("Id does not exist");
  }
  iterator->second = updatedJourneyRecord;
  return iterator->second;
}

inline std::vector<JourneyRecord> MockJourneyRepository::list() {
  std::vector<JourneyRecord> result;
  {
    std::lock_guard<std::mutex> lock(journeysMutex);
    result.reserve(journeys.size());
    for (const std::pair<int, JourneyRecord> &pair : journeys) {
      result.push_back(pair.second);
    }
  }
  return result;
}
