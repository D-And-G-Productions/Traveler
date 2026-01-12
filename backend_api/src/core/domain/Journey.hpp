#pragma once

#include "domain/Location.hpp"
#include "domain/Mode.hpp"
#include <cstdint>
#include <string>

struct Journey
{
public:
  int64_t id;
  int64_t userId;
  std::string createdAt;
  std::string updatedAt;
  std::string name;
  Location source;
  Location destination;
  std::string arrivalTime;
  Mode mode;
};
