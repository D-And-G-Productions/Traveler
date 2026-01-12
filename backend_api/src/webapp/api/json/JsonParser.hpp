#pragma once

#include "JsonParserErrors.hpp"
#include <crow/json.h>
#include <cstdint>
#include <optional>

class JsonParsing
{
public:
  static bool isFieldNull(const crow::json::rvalue &json, const std::string &field)
  {
    return json[field].t() == crow::json::type::Null;
  }

  static std::optional<std::string>
  readOptionalString(const crow::json::rvalue &json, const std::string &field)
  {
    requireField(json, field);
    if (isFieldNull(json, field))
    {
      return std::nullopt;
    }
    requireFieldIsAString(json, field);
    return json[field].s();
  }

  static std::string readString(const crow::json::rvalue &json, const std::string &field)
  {
    requireField(json, field);
    requireFieldIsAString(json, field);
    return json[field].s();
  }

  static double readDouble(const crow::json::rvalue &json, const std::string &field)
  {
    requireField(json, field);
    requireFieldIsADouble(json, field);
    return json[field].d();
  }

  static int64_t readInteger(const crow::json::rvalue &json, const std::string &field)
  {
    requireField(json, field);
    requireFieldIsADouble(json, field);
    return json[field].i();
  }

  static void requireField(const crow::json::rvalue &json, const std::string &field)
  {
    if (!json.has(field))
    {
      throw MissingFieldError(field);
    }
  }

private:
  static void requireFieldIsAString(const crow::json::rvalue &json, const std::string &field)
  {
    bool fieldIsNotAString = json[field].t() != crow::json::type::String;
    if (fieldIsNotAString)
    {
      throw IncorrectTypeError(field, "string");
    }
  }

  static void requireFieldIsADouble(const crow::json::rvalue &json, const std::string &field)
  {
    bool fieldIsNotANumber = json[field].t() != crow::json::type::Number;
    if (fieldIsNotANumber)
    {
      throw IncorrectTypeError(field, "double");
    }
  }

  static void requireFieldIsAnInteger(const crow::json::rvalue &json, const std::string &field)
  {
    bool fieldIsNotANumber = json[field].t() != crow::json::type::Number;
    if (fieldIsNotANumber)
    {
      throw IncorrectTypeError(field, "integer");
    }
  }
};
