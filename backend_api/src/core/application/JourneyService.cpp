#include "JourneyService.hpp"
#include "repository/JourneyRepository.hpp"
#include <functional>

Journey JourneyService::createJourney(const JourneyCreate &journeyCreate) {
  Journey created{};
  std::function<void()> fn = [&] { created = repository->insert(journeyCreate); };
  unitOfWork->run(fn);
  return created;
}

std::vector<Journey> JourneyService::getJourneys(const int64_t userId) {
  std::vector<Journey> journeys{};
  std::function<void()> fn = [&] { journeys = repository->selectByUserId(userId); };
  unitOfWork->run(fn);
  return journeys;
}

// TODO: Descrimination of userId should be done in the journey repository itself.
Journey JourneyService::getJourney(const int64_t userId, const int64_t journeyId) {
  Journey journey;
  std::function<void()> fn = [&] { journey = repository->selectById(journeyId); };
  unitOfWork->run(fn);
  if (journey.userId != userId) {
    throw JourneyNotFoundError("");
  }
  return journey;
}

// TODO: Descrimination of userId should be done in the journey repository itself.
Journey JourneyService::updateJourney(
    const int64_t userId,
    const int64_t journeyId,
    const JourneyCreate &journeyCreate
) {
  if (userId != journeyCreate.userId) {
    throw JourneyNotFoundError("");
  }

  Journey updatedJourney;
  std::function<void()> fn = [&] { updatedJourney = repository->update(journeyId, journeyCreate); };
  unitOfWork->run(fn);
  return updatedJourney;
}
