#pragma once

#include <stdexcept>
#include <string>

enum Mode
{
  DRIVING,
  TRANSIT,
  WALKING,
  CYCLING
};

namespace ModeUtil
{
inline Mode toMode(const std::string &string)
{
  if (string == "DRIVING")
    return Mode::DRIVING;
  if (string == "TRANSIT")
    return Mode::TRANSIT;
  if (string == "WALKING")
    return Mode::WALKING;
  if (string == "CYCLING")
    return Mode::CYCLING;
  throw std::invalid_argument("Unknown mode: " + string);
}

inline std::string toString(const Mode mode)
{
  switch (mode)
  {
  case Mode::DRIVING:
    return "DRIVING";
  case Mode::TRANSIT:
    return "TRANSIT";
  case Mode::WALKING:
    return "WALKING";
  case Mode::CYCLING:
    return "CYCLING";
  default:
    throw std::invalid_argument("Unknown mode");
  }
}
} // namespace ModeUtil
