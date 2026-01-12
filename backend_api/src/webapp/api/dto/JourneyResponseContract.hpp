#pragma once

#include "LocationResponse.hpp"
#include "domain/Mode.hpp"
#include <cstdint>
#include <string>

using std::string;

struct JourneyResponseContract
{
  int64_t id;
  string createdAt;
  string updatedAt;
  string name;
  LocationResponseContract source;
  LocationResponseContract destination;
  string arrivalTime;
  Mode mode;
};
