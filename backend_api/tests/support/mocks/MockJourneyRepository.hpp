#pragma once

#include "api/Mappings.hpp"
#include "domain/Journey.hpp"
#include "domain/JourneyCreate.hpp"
#include "repository/JourneyRepository.hpp"
#include <atomic>
#include <cstdint>
#include <mutex>
#include <string>
#include <unordered_map>
#include <utility>

class MockJourneyRepository final : public JourneyRepository {
private:
  std::mutex journeysMutex;
  std::unordered_map<int, Journey> journeys;
  std::atomic<int64_t> index{0};

public:
  Journey create(const JourneyCreate &journeyCreate) override {
    int64_t id = index.fetch_add(1, std::memory_order_relaxed);
    Journey journey = toJourney(id, journeyCreate);
    std::lock_guard<std::mutex> lock(journeysMutex);
    auto [it, ok] = journeys.insert(std::make_pair(id, journey));
    return it->second;
  }

  Journey get(int64_t id) override {
    std::lock_guard<std::mutex> lock(journeysMutex);
    auto iterator = journeys.find(id);
    if (iterator == journeys.end()) {
      throw std::invalid_argument("Id does not exist");
    }
    return iterator->second;
  }

  Journey update(int64_t id, JourneyCreate &updatedJourney) override {
    Journey newJourney = toJourney(id, updatedJourney);
    std::lock_guard<std::mutex> lock(journeysMutex);
    auto iterator = journeys.find(id);
    if (iterator == journeys.end()) {
      throw std::invalid_argument("Id does not exist");
    }
    newJourney.createdAt = iterator->second.createdAt;
    iterator->second = newJourney;
    return iterator->second;
  }

  std::vector<Journey> list() override {
    std::vector<Journey> result;
    std::lock_guard<std::mutex> lock(journeysMutex);
    result.reserve(journeys.size());
    for (auto &pair : journeys) {
      result.push_back(pair.second);
    }
    return result;
  }
};
