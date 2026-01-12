#include "MeRoutes.hpp"
#include "cpr/api.h"
#include "cpr/timeout.h"

cpr::Response MeRoutes::meGet(const std::string &token)
{
  cpr::Header authHeader = buildAuthorisationHeader(token);
  cpr::Timeout timeout = {TestConstants::TIMEOUT_MS};
  cpr::Url url = {std::format("{}/{}", serverUrl(), "me")};
  return cpr::Get(authHeader, url, timeout);
}

bool MeRoutes::isSameUser(const User &user, const UserResponseContract &ucr)
{
  return user.id == ucr.id;
}
