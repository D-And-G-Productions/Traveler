#include "support/CrowRouteFixture.h"
#include <crow/common.h>
#include <crow/http_response.h>
#include <crow/json.h>
#include <gtest/gtest.h>

class HealthGetFixture : public CrowRouteFixture {
public:
  const std::string HEALTH_URL = "/health";

  crow::response handleHealthGet() {
    crow::request request;
    request.method = crow::HTTPMethod::Get;
    request.url = HEALTH_URL;

    crow::response response;
    server->app().handle_full(request, response);
    return response;
  }
};

TEST_F(HealthGetFixture, HealthReturnsOK) {
  crow::response response = handleHealthGet();
  EXPECT_EQ(response.code, crow::OK);
}
