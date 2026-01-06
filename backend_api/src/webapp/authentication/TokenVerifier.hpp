#pragma once

#include "VerifiedToken.hpp"
#include <stdexcept>

class TokenVerificationError : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

struct TokenVerifier {
  virtual ~TokenVerifier() = default;

  virtual VerifiedToken verifyAccessToken(const std::string &javaScriptWebToken) const = 0;
};
