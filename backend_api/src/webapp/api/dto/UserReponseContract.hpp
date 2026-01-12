#pragma once

#include <crow/json.h>
#include <optional>

struct UserResponseContract
{
  std::int64_t id;
  std::optional<std::string> name;
  std::optional<std::string> telephone;
};
