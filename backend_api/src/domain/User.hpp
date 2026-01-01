#pragma once

#include <cstdint>
#include <string>

using std::string;

struct User {
  int64_t id;
  string sub;
  string createdAt;
  string updatedAt;
  string name;
  string telephone;
};
