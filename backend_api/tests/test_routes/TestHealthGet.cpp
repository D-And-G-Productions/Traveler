#include "cpr/response.h"
#include "cpr/status_codes.h"
#include "support/fixtures/HealthGet.hpp"
#include <crow/common.h>
#include <crow/http_response.h>
#include <crow/json.h>
#include <gtest/gtest.h>

TEST_F(HealthGet, HealthReturnsOK) {
  cpr::Response response = healthGet();
  EXPECT_EQ(response.status_code, cpr::status::HTTP_OK);
}
