#pragma once

#include "api/dto/JourneyCreateRequest.hpp"
#include "api/dto/JourneyResponse.hpp"
#include "api/dto/UserReponse.hpp"
#include "domain/Journey.hpp"
#include "domain/JourneyCreate.hpp"
#include "domain/Location.hpp"
#include "domain/User.hpp"
#include <cstdint>

inline UserResponse toResponse(const User &user) {
  return UserResponse{.id = user.id, .name = user.name, .telephone = user.telephone};
}

inline JourneyCreate toJourneyCreate(const int64_t userId, const JourneyCreateRequest &jcr) {
  return JourneyCreate{
      .userId = userId,
      .name = jcr.name,
      .source = jcr.source,
      .destination = jcr.destination,
      .arrivalTime = jcr.arrivalTime,
      .mode = jcr.mode
  };
}

inline LocationResponse toLocationResponse(const Location &location) {
  return LocationResponse{
      .label = location.label,
      .latitude = location.latitude,
      .longitude = location.longitude,
  };
}

inline JourneyResponse toJourneyResponse(const Journey &journey) {
  return JourneyResponse{
      .id = journey.id,
      .createdAt = journey.createdAt,
      .updatedAt = journey.updatedAt,
      .name = journey.name,
      .source = toLocationResponse(journey.source),
      .destination = toLocationResponse(journey.destination),
      .arrivalTime = journey.arrivalTime,
      .mode = journey.mode,
  };
}

inline Journey toJourney(const int64_t &id_, const JourneyCreate &journeyCreate) {
  return Journey{
      .id = id_,
      .createdAt = "PLACEHOLDER_TIME",
      .updatedAt = "PLACEHOLDER_TIME",
      .name = journeyCreate.name,
      .source = journeyCreate.source,
      .destination = journeyCreate.destination,
      .arrivalTime = journeyCreate.arrivalTime,
      .mode = journeyCreate.mode,
  };
}

inline UserResponse toUserResponse(const User &user) {
  return UserResponse{
      .id = user.id,
      .name = user.name,
      .telephone = user.telephone,
  };
}
