#pragma once

#include <crow/json.h>

inline crow::json::wvalue makeJourneyJson() {
  crow::json::wvalue json;
  json["name"] = "Test Journey";
  json["source"]["label"] = "London";
  json["source"]["latitude"] = 0.0;
  json["source"]["longitude"] = 0.0;
  json["destination"]["label"] = "Oxford";
  json["destination"]["latitude"] = 0.0;
  json["destination"]["longitude"] = 0.0;
  json["arrivalTime"] = "2025-12-24T09:30:00Z";
  json["mode"] = "driving";
  return json;
}
