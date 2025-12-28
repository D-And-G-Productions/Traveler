#pragma once

#include "authentication/TokenVerifier.hpp"
#include <crow.h>
#include <memory>

class AuthMiddleware {
public:
  // TODO: Add user id to the token. 2025-12-30
  struct context {
    bool authorized = false;
    VerifiedToken token;
  };

  void setVerifier(std::shared_ptr<const TokenVerifier> verifier);

  // Snake case is the naming scheme that Crow expects.
  void before_handle(crow::request &request, crow::response &response, context &context_);

  // Snake case is the naming scheme that Crow expects.
  void after_handle(crow::request &, crow::response &, context &);

private:
  std::shared_ptr<const TokenVerifier> verifier_;
};
