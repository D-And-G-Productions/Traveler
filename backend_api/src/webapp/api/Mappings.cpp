#include "Mappings.hpp"
#include "api/dto/LocationUpdateRequestContract.hpp"
#include "domain/Location.hpp"
#include "domain/LocationUpdate.hpp"
#include <optional>

UserResponseContract ContractMappings::toUserResponseContract(const User &u)
{
  return {
      .id = u.id,
      .name = u.name,
      .telephone = u.telephone,
  };
}

LocationCreate ContractMappings::toLocationCreate(const LocationCreateRequestContract &l)
{
  return {
      .label = l.label,
      .latitude = l.latitude,
      .longitude = l.longitude,
  };
}

JourneyCreate
ContractMappings::toJourneyCreate(const int64_t userId, const JourneyCreateRequestContract &j)
{
  return {
      .userId = userId,
      .name = j.name,
      .source = toLocationCreate(j.source),
      .destination = toLocationCreate(j.destination),
      .arrivalTime = j.arrivalTime,
      .mode = j.mode
  };
}

LocationResponseContract ContractMappings::toLocationResponseContract(const Location &l)
{
  return {
      .label = l.label,
      .latitude = l.latitude,
      .longitude = l.longitude,
  };
}

JourneyResponseContract ContractMappings::toJourneyResponseContract(const Journey &j)
{
  return {
      .id = j.id,
      .createdAt = j.createdAt,
      .updatedAt = j.updatedAt,
      .name = j.name,
      .source = toLocationResponseContract(j.source),
      .destination = toLocationResponseContract(j.destination),
      .arrivalTime = j.arrivalTime,
      .mode = j.mode,
  };
}

LocationUpdate ContractMappings::toLocationUpdate(LocationUpdateRequestContract &l)
{
  return {
      .label = l.label,
      .latitude = l.latitude,
      .longitude = l.longitude,
  };
}

std::optional<LocationUpdate>
ContractMappings::toOptionalLocationUpdate(std::optional<LocationUpdateRequestContract> &l)
{
  return l ? std::optional<LocationUpdate>(toLocationUpdate(*l)) : std::nullopt;
}

JourneyUpdate ContractMappings::toJourneyUpdate(JourneyUpdateRequestContract &j)
{
  return {
      .name = j.name,
      .source = toOptionalLocationUpdate(j.source),
      .destination = toOptionalLocationUpdate(j.destination),
      .arrivalTime = j.arrivalTime,
      .mode = j.mode,
  };
}
