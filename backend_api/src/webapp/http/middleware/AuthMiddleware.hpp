#pragma once

#include "application/UserService.hpp"
#include "authentication/TokenVerifier.hpp"
#include <crow.h>
#include <crow/middleware.h>
#include <memory>
#include <optional>

class AuthMiddleware : public crow::ILocalMiddleware
{
public:
  struct context
  {
    bool authorized = false;
    User user;
    Verification verification;
  };

  void setDeps(std::shared_ptr<TokenVerifier> tv, std::shared_ptr<UserService> us);

  void before_handle(crow::request &request, crow::response &response, context &context);

  void after_handle(crow::request &, crow::response &, context &);

  void verifyWebTokenAndUpdateContext(const std::string javaScriptWebToken, context &context);

  std::optional<std::string> getAuthorizationHeader(const crow::request &request) const;

  void verifyToken(const std::string &token, context &ctx) const;

private:
  std::shared_ptr<TokenVerifier> tokenVerifier;
  std::shared_ptr<UserService> authService;
};
