#pragma once

#include "domain/Location.hpp"
#include "domain/Mode.hpp"
#include <optional>

using std::string;

struct JourneyUpdate
{
  std::optional<std::string> name;
  std::optional<Location> source;
  std::optional<Location> destination;
  std::optional<std::string> arrivalTime;
  std::optional<Mode> mode;
};
