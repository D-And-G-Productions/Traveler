#pragma once

#include "api/Mappings.hpp"
#include "domain/Journey.hpp"
#include "domain/JourneyCreate.hpp"
#include "repository/JourneyRepository.hpp"
#include <algorithm>
#include <atomic>
#include <cstdint>
#include <mutex>
#include <string>
#include <vector>

class MockJourneyRepository final : public JourneyRepository {
private:
  mutable std::mutex journeysMutex;
  std::vector<Journey> journeys;
  std::atomic<int64_t> index{0};

  std::vector<Journey>::iterator findByIdLocked(int64_t id) {
    auto predicate = [id](const Journey &journey) { return journey.id == id; };
    return std::find_if(journeys.begin(), journeys.end(), predicate);
  }

public:
  Journey insert(const JourneyCreate &journeyCreate) override {
    int64_t id = index.fetch_add(1, std::memory_order_relaxed);
    Journey journey = toJourney(id, journeyCreate);

    std::lock_guard<std::mutex> lock(journeysMutex);
    journeys.push_back(journey);
    return journeys.back(); // copy
  }

  Journey selectById(int64_t id) override {
    std::lock_guard<std::mutex> lock(journeysMutex);
    auto it = findByIdLocked(id);
    if (it == journeys.end()) {
      throw std::invalid_argument("Id does not exist");
    }
    return *it; // copy
  }

  std::vector<Journey> selectByUserId(int64_t userId) override {
    std::vector<Journey> result{};
    std::cout << "Select By User Id 1" << std::endl;
    std::lock_guard<std::mutex> lock(journeysMutex);
    for (const auto &j : journeys) {
      if (j.userId == userId)
        result.push_back(j);
    }
    std::cout << "Select By User Id 2" << std::endl;
    return result;
  }

  Journey update(int64_t id, JourneyCreate &updatedJourney) override {
    Journey newJourney = toJourney(id, updatedJourney);

    std::lock_guard<std::mutex> lock(journeysMutex);
    auto it = findByIdLocked(id);
    if (it == journeys.end()) {
      throw std::invalid_argument("Id does not exist");
    }

    newJourney.createdAt = it->createdAt;
    *it = newJourney;
    return *it; // copy
  }

  std::vector<Journey> list() override {
    std::lock_guard<std::mutex> lock(journeysMutex);
    return journeys; // copy
  }
};
