#pragma once

#include "authentication/TokenVerifier.hpp"
#include "repository/UserRepository.hpp"
#include <string_view>

using std::string;
using std::string_view;

struct MockTokenVerifier final : public TokenVerifier {
  static constexpr string TESTING_TOKEN{"TEST_TOKEN"};
  static constexpr string TESTING_SUBJECT{"TEST_SUB"};
  static constexpr string TESTING_ISSUER{"TEST_ISSUER"};

  VerifiedToken verifyAccessToken(const string_view token) const override {
    if (token != TESTING_TOKEN) {
      throw TokenVerificationError("Invalid Token");
    }

    VerifiedToken verifiedToken;
    verifiedToken.sub = TESTING_SUBJECT;
    verifiedToken.issuer = TESTING_ISSUER;
    return verifiedToken;
  }
};
