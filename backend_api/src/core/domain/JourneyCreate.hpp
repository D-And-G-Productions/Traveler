#pragma once

#include "domain/LocationCreate.hpp"
#include "domain/Mode.hpp"
#include <cstdint>

using std::string;

struct JourneyCreate
{
  int64_t userId;
  string name;
  LocationCreate source;
  LocationCreate destination;
  string arrivalTime;
  Mode mode;
};
