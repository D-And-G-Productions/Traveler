#include "../../src/webapp/authentication/FirebaseTokenVerifier.hpp"
#include <cstdlib>
#include <iostream>

int main() {
  const char *apiKeyEnv = std::getenv("FIREBASE_API_KEY");
  const char *validTokenEnv = std::getenv("TEST_ID_TOKEN"); // Pass a fresh token here

  if (!apiKeyEnv || !validTokenEnv) {
    std::cerr << "SKIPPING: Missing API Key or Token env vars." << std::endl;
    return 0;
  }

  FirebaseTokenVerifier verifier(apiKeyEnv);

  try {
    auto result = verifier.verifyAccessToken(validTokenEnv);
    std::cout << "SUCCESS: Verified User ID: " << result.sub << std::endl;
    return 0;
  } catch (const std::exception &e) {
    std::cerr << "FAILURE: " << e.what() << std::endl;
    return 1;
  }
}
