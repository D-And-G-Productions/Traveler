#pragma once

#include <optional>
#include <string>

struct LocationUpdate
{
  std::optional<std::string> label;
  std::optional<double> latitude;
  std::optional<double> longitude;
};
