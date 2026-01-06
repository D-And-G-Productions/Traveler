#include "api/dto/JourneyResponse.hpp"
#include "api/json/JsonConversion.hpp"
#include "cpr/status_codes.h"
#include "domain/Journey.hpp"
#include "support/fixtures/JourneyIdGet.hpp"
#include <crow/common.h>
#include <crow/http_response.h>
#include <crow/json.h>
#include <cstdint>
#include <gtest/gtest.h>

TEST_F(JourneyIdGet, ExistingIdReturnsOk) {
  JourneyCreate journeyCreate{.userId = testUser.user.id};
  Journey journey = createJourney(journeyCreate);
  cpr::Response response = journeyIdGet(testUser.token, journey.id);
  EXPECT_EQ(response.status_code, cpr::status::HTTP_OK) << response.text << "\n";
}

TEST_F(JourneyIdGet, NonExistentIdReturnsNotFound) {
  const int64_t NON_EXISTENT_ID = 0;
  cpr::Response response = journeyIdGet(testUser.token, NON_EXISTENT_ID);
  EXPECT_EQ(response.status_code, crow::NOT_FOUND) << response.text << "\n";
}

TEST_F(JourneyIdGet, ResponseBodyIsParsed) {
  JourneyCreate journeyCreate{.userId = testUser.user.id, .name = "TEST_NAME"};
  Journey journey = createJourney(journeyCreate);
  cpr::Response response = journeyIdGet(testUser.token, journey.id);
  crow::json::rvalue json = crow::json::load(response.text);
  ASSERT_TRUE(json);
}

TEST_F(JourneyIdGet, ReturnJourneyWithMatchingInformation) {
  JourneyCreate journeyCreate{.userId = testUser.user.id, .name = "TEST_NAME"};
  Journey journey = createJourney(journeyCreate);
  cpr::Response response = journeyIdGet(testUser.token, journey.id);
  crow::json::rvalue json = crow::json::load(response.text);
  JourneyResponse received = api::json::fromJson<JourneyResponse>(json);
  EXPECT_EQ(received.id, journey.id);
  EXPECT_EQ(received.name, journey.name);
}
