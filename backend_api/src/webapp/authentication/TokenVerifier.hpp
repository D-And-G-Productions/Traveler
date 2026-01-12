#pragma once

#include "VerifiedToken.hpp"
#include <stdexcept>

struct TokenVerificationError : public std::runtime_error
{
  using std::runtime_error::runtime_error;
};

struct TokenVerifier
{
  virtual ~TokenVerifier() = default;

  virtual Verification verifyAccessToken(const std::string &javaScriptWebToken) const = 0;
};
