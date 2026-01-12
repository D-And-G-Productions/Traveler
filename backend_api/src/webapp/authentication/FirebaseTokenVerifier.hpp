#pragma once
#include "TokenVerifier.hpp"
#include <cpr/cpr.h>
#include <crow/json.h>
// #include <iostream>
#include <string>

class FirebaseTokenVerifier : public TokenVerifier {
public:
  explicit FirebaseTokenVerifier(std::string apiKey) : firebaseApiKey(std::move(apiKey)) {}

  VerifiedToken verifyAccessToken(const std::string &tokenAsSub) const override {
    std::string url =
        "https://identitytoolkit.googleapis.com/v1/accounts:lookup?key=" + firebaseApiKey;

    crow::json::wvalue bodyJson;
    bodyJson["idToken"] = tokenAsSub;

    cpr::Response r = cpr::Post(
        cpr::Url{url}, cpr::Header{{"Content-Type", "application/json"}}, cpr::Body{bodyJson.dump()}
    );

    if (r.status_code != 200) {
      throw TokenVerificationError("Google API Error: " + r.text);
    }

    auto responseJson = crow::json::load(r.text);

    if (!responseJson) {
      throw TokenVerificationError("Failed to parse Google response");
    }

    if (responseJson.has("users") && responseJson["users"].size() > 0) {
      std::string userId = responseJson["users"][0]["localId"].s();
      return VerifiedToken{userId};
    }

    throw TokenVerificationError("Invalid Token: No user found.");
  }

private:
  std::string firebaseApiKey;
};
