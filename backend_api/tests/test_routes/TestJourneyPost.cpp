#include "api/dto/JourneyCreateRequest.hpp"
#include "api/dto/JourneyResponse.hpp"
#include "cpr/status_codes.h"
#include "support/fixtures/JourneyPost.hpp"
#include <crow/common.h>
#include <crow/http_request.h>
#include <crow/http_response.h>
#include <crow/json.h>
#include <gtest/gtest.h>

TEST_F(JourneyPost, PostReturnsCreated) {
  JourneyCreateRequest journeyCreateRequest{};
  cpr::Response response = journeyPost(testUser.token, journeyCreateRequest);
  EXPECT_EQ(response.status_code, cpr::status::HTTP_CREATED) << response.text << "\n";
}

TEST_F(JourneyPost, PostBodyIsJson) {
  JourneyCreateRequest jcRequest{.name = "TEST_NAME"};
  cpr::Response response = journeyPost(testUser.token, jcRequest);
  crow::json::rvalue parsed = crow::json::load(response.text);
  ASSERT_TRUE(parsed);
}

TEST_F(JourneyPost, PostIsReflectedInDB) {
  JourneyCreateRequest jcRequest{.name = "TEST_NAME"};
  cpr::Response response = journeyPost(testUser.token, jcRequest);
  crow::json::rvalue json = crow::json::load(response.text);
  JourneyResponse journeyResponse = api::json::fromJson<JourneyResponse>(json);
  Journey journeyFromDB = getJourney(journeyResponse.id);
  EXPECT_EQ(journeyFromDB.id, journeyResponse.id);
}
