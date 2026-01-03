#pragma once
#include "application/UnitOfWork.hpp"
#include "domain/JourneyCreate.hpp"
#include "repository/JourneyRepository.hpp"
#include <cstdint>
#include <memory>

class JourneyService {
public:
  JourneyService(
      std::shared_ptr<UnitOfWork> unitOfWork_,
      std::shared_ptr<JourneyRepository> repository_
  ) : unitOfWork(std::move(unitOfWork_)), repository(std::move(repository_)) {}

  Journey createJourney(const JourneyCreate &journeyCreate);

  std::vector<Journey> getJourneys(const int64_t userId);

  Journey getJourney(const int64_t userId, const int64_t journeyId);

  Journey
  updateJourney(const int64_t userId, const int64_t journeyId, const JourneyCreate &journeyCreate);

private:
  std::shared_ptr<UnitOfWork> unitOfWork;
  std::shared_ptr<JourneyRepository> repository;
};
