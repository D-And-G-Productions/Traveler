#pragma once

#include "TestCommon.hpp"
#include "authentication/TokenVerifier.hpp"
#include "persistence/DBPool.hpp"
#include "persistence/StoreErrors.hpp"
#include "persistence/UserStore.hpp"
#include <format>
#include <string_view>

using std::string;
using std::string_view;

class MockTokenVerifier : public TokenVerifier
{
public:
  // TODO: Implement using a UserService.
  Verification verifyAccessToken(const std::string &token) const override
  {
    try
    {
      pqxx::connection conn{TestCommon::testDBUrl()};
      pqxx::read_transaction transaction{conn};
      UserStore userStore{transaction};
      User user = userStore.selectUser(token);
      return {.subject = token};
    }
    catch (UserDoesNotExist &)
    {
      throw TokenVerificationError(std::format("Token '{}' rejected.", token));
    }
  }
};
