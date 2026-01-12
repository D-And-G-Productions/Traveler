#pragma once

#include <format>
#include <stdexcept>

struct JsonParseError : public std::runtime_error
{
  using std::runtime_error::runtime_error;
};

struct MissingFieldError : public JsonParseError
{
  MissingFieldError(const std::string_view field) :
      JsonParseError(std::format("Field '{}' is missing", field))
  {
  }
};

struct IncorrectTypeError : public JsonParseError
{
  IncorrectTypeError(const std::string_view field, const std::string_view fieldType) :
      JsonParseError(std::format("Field '{}' must be a {}", field, fieldType))
  {
  }
};
