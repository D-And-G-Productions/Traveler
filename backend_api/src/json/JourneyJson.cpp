#include "json/JourneyJson.h"
#include "domain/Location.h"
#include "domain/Mode.h"
#include "repository/JourneyRepository.h"

#include <crow/json.h>
#include <cstdint>
#include <cstdio>
#include <stdexcept>
#include <string>

JourneyRecord JourneyJSON::toJourneyRecord(const crow::json::rvalue &json) {
  const int64_t id = readInteger(json, "id");
  Journey journey = toJourney(json);
  return JourneyRecord{id, journey};
}

Journey JourneyJSON::toJourney(const crow::json::rvalue &json) {
  const std::string name = readString(json, "name");
  const Location source = readLocation(json, "source");
  const Location destination = readLocation(json, "destination");
  const std::string arrivalTime = readString(json, "arrivalTime");
  const Mode mode = readMode(json);
  return Journey{name, source, destination, arrivalTime, mode};
}

crow::json::wvalue JourneyJSON::toJson(const JourneyRecord &journeyRecord) {
  crow::json::wvalue json = toJson(journeyRecord.value);
  json["id"] = journeyRecord.id;
  return json;
}

crow::json::wvalue JourneyJSON::toJson(const Journey &journey) {
  crow::json::wvalue json;
  json["name"] = journey.name;
  json["source"] = locationToJSON(journey.source);
  json["destination"] = locationToJSON(journey.destination);
  json["arrivalTime"] = journey.arrivalTime;
  json["mode"] = ModeUtil::toString(journey.mode);
  return json;
}

crow::json::wvalue JourneyJSON::locationToJSON(const Location &location) {
  crow::json::wvalue out;
  out["label"] = location.label;
  out["latitude"] = location.latitude;
  out["longitude"] = location.longitude;
  return out;
}

std::string JourneyJSON::readString(const crow::json::rvalue &json, const std::string &field) {
  requireField(json, field);
  bool fieldIsNotAString = json[field].t() != crow::json::type::String;
  if (fieldIsNotAString) {
    throw std::invalid_argument("Field '" + field + "' must be a string");
  }
  return std::string(json[field].s());
}

double JourneyJSON::readDouble(const crow::json::rvalue &json, const std::string &field) {
  requireField(json, field);
  bool fieldIsNotANumber = json[field].t() != crow::json::type::Number;
  if (fieldIsNotANumber) {
    throw std::invalid_argument("Field '" + field + "' must be a number");
  }
  return json[field].d();
}

int64_t JourneyJSON::readInteger(const crow::json::rvalue &json, const std::string &field) {
  requireField(json, field);
  bool fieldIsNotANumber = json[field].t() != crow::json::type::Number;
  if (fieldIsNotANumber) {
    throw std::invalid_argument("Field '" + field + "' must be a number");
  }
  return json[field].i();
}

void JourneyJSON::requireField(const crow::json::rvalue &json, const std::string &field) {
  if (!json.has(field)) throw std::invalid_argument("Missing field: " + field);
}

Location JourneyJSON::readLocation(const crow::json::rvalue &parent, const std::string &field) {
  requireField(parent, field);
  const crow::json::rvalue &location = parent[field];
  bool locationIsNotAnObject = location.t() != crow::json::type::Object;
  if (locationIsNotAnObject) {
    throw std::invalid_argument("Field '" + field + "' must be an object");
  }

  std::string label = readString(location, "label");
  double latitude = readDouble(location, "latitude");
  double longitude = readDouble(location, "longitude");
  return Location{label, latitude, longitude};
}

Mode JourneyJSON::readMode(const crow::json::rvalue &json) {
  const std::string modeString = readString(json, "mode");
  return ModeUtil::toMode(modeString);
}
