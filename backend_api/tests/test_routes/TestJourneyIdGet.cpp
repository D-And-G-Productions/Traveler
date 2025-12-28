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
  JourneyCreate journeyCreate{};
  Journey expectedRecord = server->journeyRepository->create(journeyCreate);

  cpr::Response response = journeyGetId(expectedRecord.id);
  EXPECT_EQ(response.status_code, cpr::status::HTTP_OK) << response.text << "\n";
}

TEST_F(JourneyIdGet, NonExistentIdReturnsNotFound) {
  const int64_t NON_EXISTENT_ID = 0;
  cpr::Response response = journeyGetId(NON_EXISTENT_ID);
  EXPECT_EQ(response.status_code, crow::NOT_FOUND) << response.text << "\n";
}

TEST_F(JourneyIdGet, ReturnJourneyWithMatchingId) {
  JourneyCreate journeyCreate{.name = "TEST_NAME"};
  Journey created = server->journeyRepository->create(journeyCreate);
  cpr::Response response = journeyGetId(created.id);
  crow::json::rvalue parsed = crow::json::load(response.text);

  ASSERT_TRUE(parsed);
  JourneyResponse received = api::json::fromJson<JourneyResponse>(parsed);
  EXPECT_EQ(received.id, created.id);
  EXPECT_EQ(received.name, created.name);
}
