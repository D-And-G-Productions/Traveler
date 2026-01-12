#pragma once

#include "api/dto/JourneyCreateRequestContract.hpp"
#include "api/dto/JourneyResponseContract.hpp"
#include "api/dto/JourneyUpdateRequest.hpp"
#include "api/dto/LocationCreateRequestContract.hpp"
#include "api/dto/LocationUpdateRequestContract.hpp"
#include "api/dto/UserCreate.hpp"
#include "api/dto/UserReponseContract.hpp"
#include "api/json/JsonParser.hpp"
#include "domain/Location.hpp"
#include "domain/Mode.hpp"
#include <crow/json.h>
#include <optional>
#include <string>

namespace JsonConversion
{
template <typename T>
T fromJson(const crow::json::rvalue &json);

template <typename T>
crow::json::wvalue toJson(const T &object);

template <>
inline crow::json::wvalue toJson(const Location &l)
{
  return {
      {"label", l.label},
      {"latitude", l.latitude},
      {"longitude", l.longitude},
  };
}

template <>
inline Location fromJson(const crow::json::rvalue &json)
{
  return {
      .label = JsonParsing::readString(json, "label"),
      .latitude = JsonParsing::readDouble(json, "latitude"),
      .longitude = JsonParsing::readDouble(json, "longitude"),
  };
}

template <>
inline crow::json::wvalue toJson(const LocationCreateRequestContract &l)
{
  return {
      {"label", l.label},
      {"latitude", l.latitude},
      {"longitude", l.longitude},
  };
}

template <>
inline LocationCreateRequestContract fromJson(const crow::json::rvalue &json)
{
  return {
      .label = JsonParsing::readString(json, "label"),
      .latitude = JsonParsing::readDouble(json, "latitude"),
      .longitude = JsonParsing::readDouble(json, "longitude")
  };
}

inline LocationCreateRequestContract
readLocationCreateRequestContract(const crow::json::rvalue &json, const std::string &field)
{
  JsonParsing::requireField(json, field);
  return fromJson<LocationCreateRequestContract>(json[field]);
}

template <>
inline crow::json::wvalue toJson(const JourneyCreateRequestContract &journeyCreateRequest)
{
  return {
      {"name", journeyCreateRequest.name},
      {"arrival_time", journeyCreateRequest.arrivalTime},
      {"mode", ModeUtil::toString(journeyCreateRequest.mode)},
      {"source", toJson<LocationCreateRequestContract>(journeyCreateRequest.source)},
      {"destination", toJson<LocationCreateRequestContract>(journeyCreateRequest.destination)},
  };
}

template <>
inline JourneyCreateRequestContract fromJson(const crow::json::rvalue &json)
{
  return {
      .name = JsonParsing::readString(json, "name"),
      .source = readLocationCreateRequestContract(json, "source"),
      .destination = readLocationCreateRequestContract(json, "destination"),
      .arrivalTime = JsonParsing::readString(json, "arrival_time"),
      .mode = ModeUtil::toMode(JsonParsing::readString(json, "mode")),
  };
}

template <>
inline crow::json::wvalue toJson(const LocationResponseContract &l)
{
  return {
      {"label", l.label},
      {"latitude", l.latitude},
      {"longitude", l.longitude},
  };
}

template <>
inline LocationResponseContract fromJson(const crow::json::rvalue &json)
{
  return {
      .label = JsonParsing::readString(json, "label"),
      .latitude = JsonParsing::readDouble(json, "latitude"),
      .longitude = JsonParsing::readDouble(json, "longitude")
  };
}

template <>
inline crow::json::wvalue toJson(const JourneyResponseContract &contract)
{
  return {
      {"id", contract.id},
      {"created_at", contract.createdAt},
      {"updated_at", contract.updatedAt},
      {"name", contract.name},
      {"source", toJson<LocationResponseContract>(contract.source)},
      {"destination", toJson<LocationResponseContract>(contract.destination)},
      {"arrival_time", contract.arrivalTime},
      {"mode", ModeUtil::toString(contract.mode)},
  };
}

inline LocationResponseContract
readLocationResponseContract(const crow::json::rvalue &json, const std::string &field)
{
  JsonParsing::requireField(json, field);
  return fromJson<LocationResponseContract>(json[field]);
}

template <>
inline JourneyResponseContract fromJson(const crow::json::rvalue &json)
{
  return {
      .id = JsonParsing::readInteger(json, "id"),
      .createdAt = JsonParsing::readString(json, "created_at"),
      .updatedAt = JsonParsing::readString(json, "updated_at"),
      .name = JsonParsing::readString(json, "name"),
      .source = readLocationResponseContract(json, "source"),
      .destination = readLocationResponseContract(json, "destination"),
      .arrivalTime = JsonParsing::readString(json, "arrival_time"),
      .mode = ModeUtil::toMode(JsonParsing::readString(json, "mode")),
  };
}

template <>
inline UserCreate fromJson(const crow::json::rvalue &json)
{
  std::string name = JsonParsing::readString(json, "name");
  int64_t telephone = JsonParsing::readInteger(json, "telephone");
  UserCreate userCreate{.name = name, .telephone = telephone};
  return userCreate;
}

template <typename T>
crow::json::wvalue putOptional(std::optional<T> v)
{
  return v ? crow::json::wvalue(*v) : crow::json::wvalue(nullptr);
}

template <>
inline crow::json::wvalue toJson(const UserResponseContract &ucr)
{
  return {
      {"id", ucr.id},
      {"name", putOptional(ucr.name)},
      {"telephone", putOptional(ucr.telephone)},
  };
}

template <>
inline UserResponseContract fromJson(const crow::json::rvalue &json)
{
  return UserResponseContract{
      .id = JsonParsing::readInteger(json, "id"),
      .name = JsonParsing::readOptionalString(json, "name"),
      .telephone = JsonParsing::readOptionalString(json, "telephone")
  };
}

template <>
inline crow::json::wvalue toJson(const LocationUpdateRequestContract &l)
{
  return {
      {"label", putOptional(l.label)},
      {"latitude", putOptional(l.latitude)},
      {"longitude", putOptional(l.longitude)},
  };
}

template <>
inline LocationUpdateRequestContract fromJson(const crow::json::rvalue &json)
{
  return {
      .label = JsonParsing::readOptionalString(json, "label"),
      .latitude = JsonParsing::readOptionalDouble(json, "latitude"),
      .longitude = JsonParsing::readOptionalDouble(json, "longitude"),
  };
}

namespace
{
inline crow::json::wvalue
putOptionalLocationUpdateRequestContract(std::optional<LocationUpdateRequestContract> l)
{
  return l ? crow::json::wvalue(toJson<LocationUpdateRequestContract>(*l))
           : crow::json::wvalue(nullptr);
}
} // namespace

template <>
inline crow::json::wvalue toJson(const JourneyUpdateRequestContract &j)
{
  return {
      {"name", putOptional(j.name)},
      {"arrival_time", putOptional(j.arrivalTime)},
      {"mode", putOptional(ModeUtil::toOptionalString(j.mode))},
      {"source", putOptionalLocationUpdateRequestContract(j.source)},
      {"destination", putOptionalLocationUpdateRequestContract(j.destination)},
  };
}

namespace
{
inline std::optional<LocationUpdateRequestContract>
readOptionalLocation(const crow::json::rvalue &json, const string &field)
{
  JsonParsing::requireField(json, field);
  if (JsonParsing::isFieldNull(json, field))
  {
    return std::nullopt;
  }
  return fromJson<LocationUpdateRequestContract>(json[field]);
};

inline std::optional<Mode> readOptionalMode(const crow::json::rvalue &json, const string &field)
{
  const std::optional<std::string> modeString = JsonParsing::readOptionalString(json, "mode");
  return modeString ? std::optional<Mode>{ModeUtil::toMode(*modeString)} : std::nullopt;
};
} // namespace

template <>
inline JourneyUpdateRequestContract fromJson(const crow::json::rvalue &json)
{
  return JourneyUpdateRequestContract{
      .name = JsonParsing::readOptionalString(json, "name"),
      .source = readOptionalLocation(json, "source"),
      .destination = readOptionalLocation(json, "destination"),
      .arrivalTime = JsonParsing::readOptionalString(json, "arrival_time"),
      .mode = readOptionalMode(json, "mode"),
  };
}
} // namespace JsonConversion
