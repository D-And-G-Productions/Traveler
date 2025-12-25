#pragma once
#include <stdexcept>
#include <string>

enum class Mode { DRIVING, TRANSIT, WALKING, CYCLING };

class ModeUtil {
public:
  static Mode toMode(const std::string &string) {
    if (string == "driving") return Mode::DRIVING;
    if (string == "transit") return Mode::TRANSIT;
    if (string == "walking") return Mode::WALKING;
    if (string == "cycling") return Mode::CYCLING;
    throw std::invalid_argument("Unknown mode: " + string);
  }

  static std::string toString(const Mode mode) {
    switch (mode) {
    case Mode::DRIVING:
      return "driving";
    case Mode::TRANSIT:
      return "transit";
    case Mode::WALKING:
      return "walking";
    case Mode::CYCLING:
      return "cycling";
    default:
      throw std::invalid_argument("Unknown mode");
    }
  }
};
