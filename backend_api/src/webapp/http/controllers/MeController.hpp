#pragma once

#include "api/Mappings.hpp"
#include "api/dto/UserReponse.hpp"
#include "api/json/JsonConversion.hpp"
#include "http/controllers/JourneyController.hpp"
#include "http/middleware/AuthMiddleware.hpp"
#include "repository/UserRepository.hpp"
#include <crow/app.h>
#include <crow/common.h>
#include <crow/http_request.h>
#include <crow/http_response.h>
#include <memory>
#include <string>

using crow::HTTPMethod::GET;
using context = AuthMiddleware::context;

class MeController {
private:
  std::shared_ptr<UserRepository> userRepository;

public:
  MeController(std::shared_ptr<UserRepository> userRepository_) : userRepository(userRepository_) {}

  void registerRoutes(crow::App<AuthMiddleware> &app) {
    CROW_ROUTE(app, "/me").methods(GET)([this, &app](const crow::request &request) {
      context &context = app.get_context<AuthMiddleware>(request);
      return queryMe(context, request);
    });
  }

private:
  crow::response queryMe(context &context, const crow::request &request) {
    string subject = context.verifiedToken.sub;
    User user = userRepository->get(subject);
    UserResponse userResponse = toUserResponse(user);
    crow::json::wvalue json = api::json::toJson(userResponse);
    return crow::response{crow::OK, "application/json", json.dump()};
  };
};
