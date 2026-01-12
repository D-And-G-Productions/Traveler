#include "HealthController.hpp"
#include <crow/common.h>
#include <crow/http_response.h>

using crow::HTTPMethod::GET;

void HealthController::registerRoutes(TravelerApp &app)
{
  CROW_ROUTE(app, "/health").methods(GET)([]() { return crow::response(crow::OK); });
}
