#pragma once

#include <crow/json.h>
#include <cstdint>
#include <string>

#include "domain/Journey.h"
#include "domain/Location.h"
#include "domain/Mode.h"
#include "repository/JourneyRepository.h"

class JourneyJSON {
public:
  static JourneyRecord toJourneyRecord(const crow::json::rvalue &json);

  static Journey toJourney(const crow::json::rvalue &json);

  static crow::json::wvalue toJson(const JourneyRecord &journeyRecord);

  static crow::json::wvalue toJson(const Journey &journey);

private:
  static crow::json::wvalue locationToJSON(const Location &location);

  static std::string readString(const crow::json::rvalue &json, const std::string &field);

  static double readDouble(const crow::json::rvalue &json, const std::string &field);

  static int64_t readInteger(const crow::json::rvalue &json, const std::string &field);

  static void requireField(const crow::json::rvalue &json, const std::string &field);

  static Location readLocation(const crow::json::rvalue &parent, const std::string &field);

  static Mode readMode(const crow::json::rvalue &json);
};
