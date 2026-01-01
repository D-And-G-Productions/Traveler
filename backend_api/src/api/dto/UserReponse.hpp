#pragma once

#include <crow/json.h>
#include <cstdint>
#include <string>

using std::string;

struct UserResponse {
  int64_t id;
  string name;
  string telephone;
};
