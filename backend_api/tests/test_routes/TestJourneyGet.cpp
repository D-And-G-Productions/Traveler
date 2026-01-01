#include "cpr/response.h"
#include "cpr/status_codes.h"
#include "domain/JourneyCreate.hpp"
#include "domain/User.hpp"
#include "support/fixtures/JourneyGet.hpp"
#include "support/fixtures/RouteFixture.hpp"
#include <crow/common.h>
#include <crow/http_response.h>
#include <crow/json.h>
#include <gtest/gtest.h>
#include <string>

using std::string;

TEST_F(JourneyGet, ReturnsOK) {
  cpr::Response response = journeyGet(testUser.token);
  EXPECT_EQ(response.status_code, cpr::status::HTTP_OK);
}

TEST_F(JourneyGet, EmptyResponseBodyIsParsed) {
  cpr::Response response = journeyGet(testUser.token);
  crow::json::rvalue json = crow::json::load(response.text);
  EXPECT_TRUE(json);
}

TEST_F(JourneyGet, NonEmptyResponseBodyIsParsed) {
  JourneyCreate journeyCreate{.userId = testUser.user.id};
  createJourney(journeyCreate);
  cpr::Response response = journeyGet(testUser.token);
  crow::json::rvalue json = crow::json::load(response.text);
  EXPECT_TRUE(json);
}

TEST_F(JourneyGet, ReceiveZeroJourneys) {
  cpr::Response response = journeyGet(testUser.token);
  crow::json::rvalue journeysJson = crow::json::load(response.text);
  EXPECT_EQ(journeysJson.size(), 0);
}

TEST_F(JourneyGet, ReceiveOneJourney) {
  JourneyCreate journeyCreate{.userId = testUser.user.id};
  createJourney(journeyCreate);
  cpr::Response response = journeyGet(testUser.token);
  crow::json::rvalue journeysJson = crow::json::load(response.text);
  EXPECT_EQ(journeysJson.size(), 1);
}

TEST_F(JourneyGet, ReceiveTwoJourneys) {
  JourneyCreate journeyCreate{};
  createJourney(journeyCreate);
  createJourney(journeyCreate);
  cpr::Response response = journeyGet(testUser.token);
  crow::json::rvalue journeysJson = crow::json::load(response.text);
  EXPECT_EQ(journeysJson.size(), 2);
}

TEST_F(JourneyGet, UserReceivesOnlyTheirJourneys) {
  TestUser differentUser = createTestUser("DIFFERENT USER");
  JourneyCreate journeyCreate{.userId = testUser.user.id};
  createJourney(journeyCreate);

  cpr::Response testUserResponse = journeyGet(testUser.token);
  cpr::Response differentUserResponse = journeyGet(differentUser.token);
  crow::json::rvalue testUserJourneysJson = crow::json::load(testUserResponse.text);
  crow::json::rvalue differentUserJourneysJson = crow::json::load(differentUserResponse.text);
  EXPECT_EQ(testUserJourneysJson.size(), 1);
  EXPECT_EQ(differentUserJourneysJson.size(), 0);
}
