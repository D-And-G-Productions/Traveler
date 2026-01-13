#pragma once

#include "persistence/EnvVariables.hpp"
#include <string_view>

struct TestConstants
{
  TestConstants() = delete;
  static constexpr int TEST_PORT = 45451;
  static constexpr int TIMEOUT_MS = 2000;
  static constexpr std::string_view LOCAL_HOST = "http://127.0.0.1";
  static constexpr std::string_view LOCAL_ADDRESS = "127.0.0.1";

  static const std::string TEST_DATABASE_URL() { return EnvVariables::getVar("DATABASE_URL"); }
};
