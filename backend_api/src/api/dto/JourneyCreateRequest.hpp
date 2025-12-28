#pragma once

#include "domain/Location.hpp"
#include "domain/Mode.hpp"
#include <crow/json.h>
#include <string>

using std::string;

struct JourneyCreateRequest {
  string name;
  Location source;
  Location destination;
  string arrivalTime;
  Mode mode;
};
