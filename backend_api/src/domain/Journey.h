#pragma once

#include "domain/Location.h"
#include "domain/Mode.h"
#include <string>

struct Journey {
  std::string name;
  Location source;
  Location destination;
  std::string arrivalTime;
  Mode mode;
};
