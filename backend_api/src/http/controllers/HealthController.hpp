#include <crow/app.h>
#include <crow/common.h>
#include <crow/http_request.h>
#include <crow/http_response.h>

class HealthController {
public:
  void registerRoutes(crow::SimpleApp &app);

private:
  crow::response queryHealth();
};
