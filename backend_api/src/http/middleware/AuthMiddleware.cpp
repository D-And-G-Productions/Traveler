#include "AuthMiddleware.hpp"

void AuthMiddleware::after_handle(crow::request &, crow::response &, context &) {}

void AuthMiddleware::before_handle(
    crow::request &request, crow::response &response, context &context_
) {
  const std::string auth = request.get_header_value("Authorization");
  if (auth.rfind("Bearer ", 0) != 0) {
    response.code = 401;
    response.write("Missing bearer token");
    response.end();
    return;
  }

  const std::string javaScriptWebToken = auth.substr(7);
  try {
    context_.token = verifier_->verifyAccessToken(javaScriptWebToken);
    context_.authorized = true;
  } catch (const TokenVerificationError &) {
    response.code = 401;
    response.end();
    return;
  }
}

void AuthMiddleware::setVerifier(std::shared_ptr<const TokenVerifier> verifier) {
  verifier_ = std::move(verifier);
}
