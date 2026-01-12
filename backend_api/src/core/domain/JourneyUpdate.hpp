#pragma once

#include "domain/LocationUpdate.hpp"
#include "domain/Mode.hpp"
#include <optional>

using std::string;

struct JourneyUpdate
{
  std::optional<std::string> name;
  std::optional<LocationUpdate> source;
  std::optional<LocationUpdate> destination;
  std::optional<std::string> arrivalTime;
  std::optional<Mode> mode;
};
