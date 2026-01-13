#pragma once

#include <stdexcept>
#include <string>

namespace EnvVariables
{
static bool isNonExistentOrEmpty(const char *v) { return !(v && *v); }

static std::string getVar(const char *key)
{
  const char *variable = std::getenv(key);
  if (isNonExistentOrEmpty(variable))
  {
    throw std::runtime_error(std::string{"Missing env variable: "} + key);
  }
  return std::string{variable};
}
} // namespace EnvVariables
