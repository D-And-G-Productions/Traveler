#pragma once

#include "authentication/TokenVerifier.hpp"
#include <string_view>

using std::string;
using std::string_view;

class MockTokenVerifier : public TokenVerifier {
public:
  void acceptAll() { isRejecting = false; }

  void rejectAll() { isRejecting = true; }

  VerifiedToken verifyAccessToken(const std::string &tokenAsSub) const override {
    if (isRejecting)
      throw TokenVerificationError{"Invalid Error"};
    return VerifiedToken{.sub = tokenAsSub};
  }

private:
  bool isRejecting{false};
};
