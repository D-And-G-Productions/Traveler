#include "AuthMiddleware.hpp"
#include "repository/UserRepositoryErrors.hpp"

void AuthMiddleware::setVerifier(std::shared_ptr<const TokenVerifier> verifier_) {
  verifier = std::move(verifier_);
}

void AuthMiddleware::setUserRepository(std::shared_ptr<UserRepository> userRepository_) {
  userRepository = std::move(userRepository_);
}

void AuthMiddleware::after_handle(crow::request &, crow::response &, context &) {}

void AuthMiddleware::before_handle(
    crow::request &request,
    crow::response &response,
    context &context
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
    context.verifiedToken = verifier->verifyAccessToken(javaScriptWebToken);
    context.authorized = true;
  } catch (const TokenVerificationError &) {
    response.code = 401;
    response.end();
    return;
  }

  User user;
  try {
    user = userRepository->get(context.verifiedToken.sub);
  } catch (UserDoesNotExist &error) {
    user = userRepository->insert(context.verifiedToken.sub);
  }
  context.userId = user.id;
}
