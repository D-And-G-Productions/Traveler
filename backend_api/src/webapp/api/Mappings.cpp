#include "Mappings.hpp"

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

JourneyUpdate ContractMappings::toJourneyUpdate(JourneyUpdateRequestContract &j)
{
  return {
      .name = j.name,
      .source = j.source,
      .destination = j.destination,
      .arrivalTime = j.arrivalTime,
      .mode = j.mode,
  };
}
