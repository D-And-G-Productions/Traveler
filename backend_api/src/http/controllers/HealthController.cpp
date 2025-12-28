#include "HealthController.hpp"

void HealthController::registerRoutes(crow::SimpleApp &app) {
  CROW_ROUTE(app, "/health").methods(crow::HTTPMethod::Get)([this]() { return queryHealth(); });
};

crow::response HealthController::queryHealth() { return crow::response(crow::OK); };
