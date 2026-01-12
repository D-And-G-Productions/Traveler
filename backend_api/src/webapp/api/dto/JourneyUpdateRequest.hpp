#pragma once

#include "api/dto/LocationUpdateRequestContract.hpp"
#include "domain/Mode.hpp"
#include <optional>

struct JourneyUpdateRequestContract
{
  std::optional<std::string> name;
  std::optional<LocationUpdateRequestContract> source;
  std::optional<LocationUpdateRequestContract> destination;
  std::optional<std::string> arrivalTime;
  std::optional<Mode> mode;
};
