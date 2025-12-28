#pragma once

#include "domain/Location.hpp"
#include "domain/Mode.hpp"
#include <cstdint>

using std::string;

struct JourneyCreate {
  int64_t userId;
  string name;
  Location source;
  Location destination;
  string arrivalTime;
  Mode mode;
};
