#include "JourneyStore.hpp"
#include "domain/JourneyCreate.hpp"
#include "persistence/Queries.hpp"
#include "persistence/Store.hpp"
#include "persistence/StoreErrors.hpp"
#include <cstdint>
#include <pqxx/internal/statement_parameters.hxx>

namespace
{
void requireJourneyIsFound(const pqxx::result &r, const int64_t journeyId, const int64_t userId)
{
  if (r.empty())
  {
    const std::string errorMessage =
        std::format("Journey with id '{}' AND user_id '{}' not found.", journeyId, userId);
    throw JourneyNotFoundError(errorMessage);
  }
}

void requireJourneyIsFound(const pqxx::result &r, const int64_t journeyId)
{
  if (r.empty())
  {
    const std::string errorMessage = std::format("Journey with id '{}' not found.", journeyId);
    throw JourneyNotFoundError(errorMessage);
  }
}

Location rowToLocation(const pqxx::row &row, std::string which)
{
  const std::string latitude = which + "_latitude";
  const std::string longitude = which + "_longitude";
  return {
      .latitude = row[latitude].as<double>(),
      .longitude = row[longitude].as<double>(),
  };
};

Journey rowToJourney(const pqxx::row &row)
{
  return {
      .id = row["id"].as<int64_t>(),
      .userId = row["user_id"].as<int64_t>(),
      .createdAt = row["created_at"].as<std::string>(),
      .updatedAt = row["updated_at"].as<std::string>(),
      .source = rowToLocation(row, "source"),
      .destination = rowToLocation(row, "destination"),
      .arrivalTime = row["arrival_time"].as<std::string>(),
      .mode = ModeUtil::toMode(row["mode"].as<std::string>()),
  };
}

std::vector<Journey> mapJourneys(const pqxx::result &result)
{
  std::vector<Journey> journeys;
  journeys.reserve(result.size());
  for (const pqxx::row &row : result)
  {
    journeys.push_back(rowToJourney(row));
  }
  return journeys;
}
} // namespace

std::vector<Journey> JourneyStore::selectJourneysByUserId(const int64_t userId)
{
  pqxx::params params{userId};
  pqxx::result result = transaction.exec(Queries::selectJourneysByUserIdQuery, params);
  return mapJourneys(result);
}

namespace
{
pqxx::params buildInsertJourneyParameters(const JourneyCreate &jc)
{
  return {
      jc.userId,
      jc.arrivalTime,
      ModeUtil::toString(jc.mode),
      jc.source.label,
      jc.source.latitude,
      jc.source.longitude,
      jc.destination.label,
      jc.destination.latitude,
      jc.destination.longitude,
  };
}
} // namespace

Journey JourneyStore::insertJourney(const JourneyCreate &jc)
{
  pqxx::params params = buildInsertJourneyParameters(jc);
  pqxx::result result = transaction.exec(Queries::insertJourney, params);
  return rowToJourney(result.one_row());
}

Journey JourneyStore::selectJourney(const int64_t journeyId)
{
  pqxx::params params{journeyId};
  pqxx::result result = transaction.exec(Queries::selectJourneyById, params);
  requireJourneyIsFound(result, journeyId);
  return rowToJourney(result.one_row());
}

Journey JourneyStore::selectJourney(const int64_t journeyId, const int64_t userId)
{
  pqxx::params params{journeyId, userId};
  pqxx::result result = transaction.exec(Queries::selectJourneyByIdAndUserId, params);
  requireJourneyIsFound(result, journeyId, userId);
  return rowToJourney(result.one_row());
}

void JourneyStore::deleteJourney(const int64_t journeyId, const int64_t userId)
{
  pqxx::params params{journeyId, userId};
  pqxx::result result = transaction.exec(Queries::deleteJourneyByIdAndUserIdQuery, params);
  requireJourneyIsFound(result, journeyId, userId);
}

namespace
{
pqxx::params buildUpdateJourneyParameters(
    const int64_t journeyId,
    const int64_t userId,
    const JourneyUpdate &update
)
{
  const std::optional<double> sourceLatitude =
      update.source ? std::optional<double>{update.source->latitude} : std::nullopt;
  const std::optional<double> sourceLongitude =
      update.source ? std::optional<double>{update.source->longitude} : std::nullopt;

  const std::optional<double> destinationLatitude =
      update.destination ? std::optional<double>{update.destination->latitude} : std::nullopt;
  const std::optional<double> destinationLongitude =
      update.destination ? std::optional<double>{update.destination->longitude} : std::nullopt;

  const std::optional<std::string> modeString =
      update.mode ? std::optional<std::string>{ModeUtil::toString(*update.mode)} : std::nullopt;

  return {
      journeyId,
      userId,
      sourceLatitude,
      sourceLongitude,
      destinationLatitude,
      destinationLongitude,
      update.arrivalTime,
      modeString,
  };
}
} // namespace

Journey JourneyStore::updateJourney(
    const int64_t journeyId,
    const int64_t userId,
    const JourneyUpdate &update
)
{
  pqxx::params params = buildUpdateJourneyParameters(journeyId, userId, update);
  pqxx::result result = transaction.exec(Queries::updateJourneyByIdAndUserIdQuery, params);
  requireJourneyIsFound(result, journeyId, userId);
  return rowToJourney(result.one_row());
}
