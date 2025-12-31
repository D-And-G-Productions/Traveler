#pragma once

#include "LocationResponse.hpp"
#include "domain/Mode.hpp"
#include <cstdint>
#include <string>

using std::string;

struct JourneyResponse {
  int64_t id;
  string createdAt;
  string updatedAt;
  string name;
  LocationResponse source;
  LocationResponse destination;
  string arrivalTime;
  Mode mode;
};
