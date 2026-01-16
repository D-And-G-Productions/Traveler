#pragma once

#include <cstdint>
#include <optional>
#include <string>

using std::string;

struct User
{
  int64_t id;
  string subject;
  string createdAt;
  string updatedAt;
  std::optional<string> name;
  std::optional<string> telephone;

  bool operator==(const User &) const = default;
};
