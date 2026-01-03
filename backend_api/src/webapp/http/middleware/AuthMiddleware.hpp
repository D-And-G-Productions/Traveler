#pragma once

#include "authentication/TokenVerifier.hpp"
#include "repository/UserRepository.hpp"
#include <crow.h>
#include <memory>

class AuthMiddleware {
public:
  struct context {
    bool authorized = false;
    VerifiedToken verifiedToken;
    int64_t userId;
  };

  void setVerifier(std::shared_ptr<const TokenVerifier> verifier_);

  void setUserRepository(std::shared_ptr<UserRepository> userRepository_);

  // Snake case is the naming scheme that Crow expects.
  void before_handle(crow::request &request, crow::response &response, context &context_);

  // Snake case is the naming scheme that Crow expects.
  void after_handle(crow::request &, crow::response &, context &);

private:
  std::shared_ptr<const TokenVerifier> verifier;

  std::shared_ptr<UserRepository> userRepository;
};
