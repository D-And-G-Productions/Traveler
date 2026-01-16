#pragma once

#include "VerifiedToken.hpp"
#include <format>
#include <stdexcept>

struct TokenVerificationError : public std::runtime_error
{
  TokenVerificationError(const std::string_view &token) :
      std::runtime_error::runtime_error(formatErrorMessage(token))
  {
  }

  static std::string formatErrorMessage(const std::string_view &token)
  {
    return std::format("Token '{}' rejected.", token);
  }
};

struct TokenVerifier
{
  virtual ~TokenVerifier() = default;

  virtual Verification verifyAccessToken(const std::string &javaScriptWebToken) const = 0;
};
