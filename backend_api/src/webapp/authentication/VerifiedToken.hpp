#pragma once

#include <string>
#include <vector>

struct Verification
{
  std::string subject;
  std::string issuer;
  std::vector<std::string> scopes;
  std::vector<std::string> groups;
};
