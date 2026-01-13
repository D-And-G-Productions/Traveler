#pragma once

#include "application/UserService.hpp"
#include "authentication/TokenVerifier.hpp"
#include "persistence/DBPool.hpp"
#include "persistence/StoreErrors.hpp"
#include <memory>
#include <string_view>
#include <utility>

using std::string;
using std::string_view;

class MockTokenVerifier : public TokenVerifier
{
public:
  MockTokenVerifier(std::shared_ptr<DBPool> dbPool) : pool(std::move(dbPool)) {}

  Verification verifyAccessToken(const std::string &token) const override
  {
    try
    {
      UserService service{pool};
      User existingUser = service.getUser(token);
      return {.subject = existingUser.subject};
    }
    catch (UserDoesNotExist &)
    {
      throw TokenVerificationError(token);
    }
  }

private:
  std::shared_ptr<DBPool> pool;
};
