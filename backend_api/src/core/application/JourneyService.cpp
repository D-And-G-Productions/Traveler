#include "JourneyService.hpp"
#include "repository/JourneyRepository.hpp"

Journey JourneyService::createJourney(const JourneyCreate &journeyCreate) {
  Journey created{};
  unitOfWork->run([&] { created = repository->insert(journeyCreate); });
  return created;
}

std::vector<Journey> JourneyService::getJourneys(const int64_t userId) {
  return repository->selectByUserId(userId);
}

Journey JourneyService::getJourney(const int64_t userId, const int64_t journeyId) {
  // TODO: Implement this by a search for a record matching both Ids.

  try {
    Journey journey = repository->selectById(journeyId);
    if (journey.userId != userId)
      throw std::invalid_argument("");

    return journey;
  } catch (std::invalid_argument &error) {
    throw JourneyNotFoundError("");
  }
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
  Journey updatedJourney = repository->update(journeyId, journeyCreate);
  return updatedJourney;
}
