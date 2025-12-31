#pragma once

#include "http/middleware/AuthMiddleware.hpp"
#include <crow/app.h>
#include <crow/common.h>
#include <crow/http_request.h>
#include <crow/http_response.h>

class HealthController {
public:
  void registerRoutes(crow::App<AuthMiddleware> &app) {
    CROW_ROUTE(app, "/health").methods(crow::HTTPMethod::Get)([this]() { return queryHealth(); });
  }

private:
  crow::response queryHealth() { return crow::response(crow::OK); }
};
