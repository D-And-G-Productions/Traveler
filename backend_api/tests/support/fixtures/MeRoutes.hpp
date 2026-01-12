#pragma once

#include "api/dto/UserReponseContract.hpp"
#include "fixtures/RouteFixture.hpp"
#include <cpr/response.h>

class MeRoutes : public RouteFixture
{
protected:
  cpr::Response meGet(const std::string &token);

  bool isSameUser(const User &user, const UserResponseContract &ucr);
};
