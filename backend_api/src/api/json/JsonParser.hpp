#pragma once

#include <crow/json.h>
#include <cstdint>
#include <format>
#include <stdexcept>
#include <string>

using crow::json::rvalue;
using crow::json::wvalue;
using std::string;

struct JsonParseError : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

class JsonParsing {
public:
  static std::string readString(const rvalue &json, const string &field) {
    requireField(json, field);
    requireFieldIsAString(json, field);
    return json[field].s();
  }

  static double readDouble(const rvalue &json, const string &field) {
    requireField(json, field);
    requireFieldIsADouble(json, field);
    return json[field].d();
  }

  static int64_t readInteger(const rvalue &json, const string &field) {
    requireField(json, field);
    requireFieldIsADouble(json, field);
    return json[field].i();
  }

  static void requireField(const rvalue &json, const string &field) {

    if (!json.has(field)) {
      string errorMessage = std::format("Missing field '{}'", field);
      throw JsonParseError(errorMessage);
    }
  }

private:
  static void requireFieldIsAString(const rvalue &json, const string &field) {
    bool fieldIsNotAString = json[field].t() != crow::json::type::String;
    if (fieldIsNotAString) {
      string errorMessage = std::format("Field '{}' must be a string", field);
      throw JsonParseError(errorMessage);
    }
  }

  static void requireFieldIsADouble(const rvalue &json, const string &field) {
    bool fieldIsNotANumber = json[field].t() != crow::json::type::Number;
    if (fieldIsNotANumber) {
      string errorMessage = std::format("Field '{}' must be a double", field);
      throw JsonParseError(errorMessage);
    }
  }

  static void requireFieldIsAnInteger(const rvalue &json, const std::string &field) {
    bool fieldIsNotANumber = json[field].t() != crow::json::type::Number;
    if (fieldIsNotANumber) {
      string errorMessage = std::format("Field '{}' must be an integer", field);
      throw JsonParseError(errorMessage);
    }
  }
};
