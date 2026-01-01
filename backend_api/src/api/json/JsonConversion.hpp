#pragma once

#include "api/dto/JourneyCreateRequest.hpp"
#include "api/dto/JourneyResponse.hpp"
#include "api/dto/UserCreate.hpp"
#include "api/dto/UserReponse.hpp"
#include "api/json/JsonParser.hpp"
#include "domain/Location.hpp"
#include "domain/Mode.hpp"
#include <crow/json.h>
#include <string>

namespace api::json {
template <typename T>
T fromJson(const crow::json::rvalue &json);

template <typename T>
crow::json::wvalue toJson(const T &object);

template <>
inline crow::json::wvalue toJson(const Location &location) {
  crow::json::wvalue json;
  json["label"] = location.label;
  json["latitude"] = location.latitude;
  json["longitude"] = location.longitude;
  return json;
}

template <>
inline Location fromJson(const crow::json::rvalue &json) {
  Location locationCreate;
  locationCreate.label = JsonParsing::readString(json, "label");
  locationCreate.latitude = JsonParsing::readDouble(json, "latitude");
  locationCreate.longitude = JsonParsing::readDouble(json, "longitude");
  return locationCreate;
}

template <>
inline crow::json::wvalue toJson(const LocationResponse &locationResponse) {
  crow::json::wvalue json;
  json["label"] = locationResponse.label;
  json["latitude"] = locationResponse.latitude;
  json["longitude"] = locationResponse.longitude;
  return json;
}

template <>
inline LocationResponse fromJson(const crow::json::rvalue &json) {
  return LocationResponse{
      .label = JsonParsing::readString(json, "label"),
      .latitude = JsonParsing::readDouble(json, "latitude"),
      .longitude = JsonParsing::readDouble(json, "longitude")
  };
}

template <>
inline crow::json::wvalue toJson(const JourneyCreateRequest &journeyCreateRequest) {
  crow::json::wvalue json;
  json["name"] = journeyCreateRequest.name;
  json["arrival_time"] = journeyCreateRequest.arrivalTime;
  json["mode"] = ModeUtil::toString(journeyCreateRequest.mode);
  json["source"] = toJson(journeyCreateRequest.source);
  json["destination"] = toJson(journeyCreateRequest.destination);
  return json;
}

template <>
inline JourneyCreateRequest fromJson(const crow::json::rvalue &json) {
  JourneyCreateRequest journeyCreateRequest;

  JsonParsing::requireField(json, "source");
  journeyCreateRequest.source = fromJson<Location>(json["source"]);
  JsonParsing::requireField(json, "destination");
  journeyCreateRequest.destination = fromJson<Location>(json["destination"]);

  journeyCreateRequest.name = JsonParsing::readString(json, "name");
  journeyCreateRequest.arrivalTime = JsonParsing::readString(json, "arrival_time");
  journeyCreateRequest.mode = ModeUtil::toMode(JsonParsing::readString(json, "mode"));

  return journeyCreateRequest;
}

template <>
inline crow::json::wvalue toJson(const JourneyResponse &journeyResponse) {
  crow::json::wvalue json;
  json["id"] = journeyResponse.id;
  json["created_at"] = journeyResponse.createdAt;
  json["updated_at"] = journeyResponse.updatedAt;
  json["name"] = journeyResponse.name;
  json["source"] = toJson(journeyResponse.source);
  json["destination"] = toJson(journeyResponse.destination);
  json["arrival_time"] = journeyResponse.arrivalTime;
  json["mode"] = ModeUtil::toString(journeyResponse.mode);
  return json;
}

template <>
inline JourneyResponse fromJson<JourneyResponse>(const crow::json::rvalue &json) {
  JsonParsing::requireField(json, "source");
  crow::json::rvalue sourceJson = json["source"];
  LocationResponse source = fromJson<LocationResponse>(sourceJson);

  JsonParsing::requireField(json, "destination");
  crow::json::rvalue destinationJson = json["destination"];
  LocationResponse destination = fromJson<LocationResponse>(destinationJson);

  return JourneyResponse{
      .id = JsonParsing::readInteger(json, "id"),
      .createdAt = JsonParsing::readString(json, "created_at"),
      .updatedAt = JsonParsing::readString(json, "updated_at"),
      .name = JsonParsing::readString(json, "name"),
      .source = source,
      .destination = destination,
      .arrivalTime = JsonParsing::readString(json, "arrival_time"),
      .mode = ModeUtil::toMode(JsonParsing::readString(json, "mode")),
  };
}

template <>
inline UserCreate fromJson(const crow::json::rvalue &json) {
  std::string name = JsonParsing::readString(json, "name");
  int64_t telephone = JsonParsing::readInteger(json, "telephone");
  UserCreate userCreate{.name = name, .telephone = telephone};
  return userCreate;
}

template <>
inline crow::json::wvalue toJson(const UserResponse &userResponse) {
  crow::json::wvalue json;
  json["id"] = userResponse.id;
  json["name"] = userResponse.name;
  json["telephone"] = userResponse.telephone;
  return json;
}

inline UserResponse fromJson(const crow::json::rvalue &json) {
  return UserResponse{
      .id = JsonParsing::readInteger(json, "id"),
      .name = JsonParsing::readString(json, "name"),
      .telephone = JsonParsing::readString(json, "telephone")
  };
}
} // namespace api::json
