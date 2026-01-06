#pragma once

#include "domain/Location.hpp"
#include "domain/Mode.hpp"

using std::string;

struct JourneyCreate {
  string name;
  Location source;
  Location destination;
  string arrivalTime;
  Mode mode;
};
