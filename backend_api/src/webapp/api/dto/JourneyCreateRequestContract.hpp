#pragma once

#include "api/dto/LocationCreateRequestContract.hpp"
#include "domain/Mode.hpp"
#include <crow/json.h>
#include <string>

using std::string;

struct JourneyCreateRequestContract
{
  string name;
  LocationCreateRequestContract source;
  LocationCreateRequestContract destination;
  string arrivalTime;
  Mode mode;
};
