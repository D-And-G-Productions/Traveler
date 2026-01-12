#pragma once

#include "domain/Location.hpp"
#include "domain/Mode.hpp"
#include <optional>

struct JourneyUpdateRequestContract
{
  std::optional<std::string> name;
  std::optional<Location> source;
  std::optional<Location> destination;
  std::optional<std::string> arrivalTime;
  std::optional<Mode> mode;
};
