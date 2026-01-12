#include "AuthMiddleware.hpp"
#include "authentication/VerifiedToken.hpp"
#include "persistence/StoreErrors.hpp"

namespace
{
constexpr std::string_view BEARER_PREFIX = "Bearer ";

struct AuthHeaderInvalid : public std::runtime_error
{
  using std::runtime_error::runtime_error;
};

void setResponseToFailState(crow::response &r)
{
  r.code = crow::UNAUTHORIZED;
  r.end();
}

void setResponseToFailState(crow::response &r, const std::string_view s)
{
  r.code = crow::UNAUTHORIZED;
  r.body = s;
  r.end();
}

void requireAuthValueIsValid(const std::string_view v)
{
  bool prefixedByBearer = v.rfind(BEARER_PREFIX, 0) == 0;
  if (!prefixedByBearer)
  {
    const std::string message = std::format("Auth value is not prefixed with '{}'", BEARER_PREFIX);
    throw AuthHeaderInvalid(message);
  }
}

std::string extractToken(const std::string_view s)
{
  return static_cast<std::string>(s.substr(BEARER_PREFIX.size()));
}
} // namespace

void AuthMiddleware::setDeps(std::shared_ptr<TokenVerifier> tv, std::shared_ptr<UserService> us)
{
  tokenVerifier = std::move(tv);
  authService = std::move(us);
}

void AuthMiddleware::before_handle(crow::request &request, crow::response &response, context &ctx)
{
  try
  {
    const std::string value = request.get_header_value("Authorization");
    requireAuthValueIsValid(value);
    const std::string token = extractToken(value);
    verifyToken(token, ctx);
  }
  catch (const AuthHeaderInvalid &e)
  {
    setResponseToFailState(response, e.what());
  }
  catch (const TokenVerificationError &)
  {
    setResponseToFailState(response);
  }
  catch (const UserDoesNotExist &)
  {
    setResponseToFailState(response);
  }
}

void AuthMiddleware::after_handle(crow::request &, crow::response &, context &) {}

void AuthMiddleware::verifyToken(const std::string &token, context &ctx) const
{
  ctx.verification = tokenVerifier->verifyAccessToken(token);
  ctx.user = authService->getUser(ctx.verification.subject);
}
