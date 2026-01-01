#include <cstdint>
#include <string>
#include <vector>

using std::string, std::vector;

struct VerifiedToken {
  string sub;
  string issuer;
  vector<string> scopes;
  vector<string> groups;
};
