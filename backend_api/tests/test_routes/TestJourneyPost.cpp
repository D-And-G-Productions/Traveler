#include "api/dto/JourneyCreateRequest.hpp"
#include "api/dto/JourneyResponse.hpp"
#include "cpr/status_codes.h"
#include "support/fixtures/JourneyPost.hpp"
#include <crow/common.h>
#include <crow/http_request.h>
#include <crow/http_response.h>
#include <crow/json.h>
#include <gtest/gtest.h>

TEST_F(JourneyPost, PostReturnsOk) {
  JourneyCreateRequest journeyCreate{};
  cpr::Response response = journeyPost(journeyCreate);
  EXPECT_EQ(response.status_code, cpr::status::HTTP_CREATED) << response.text << "\n";
}

TEST_F(JourneyPost, PostReturnsJourneyWithId) {
  JourneyCreateRequest journeyCreate{.name = "TEST_NAME"};
  cpr::Response response = journeyPost(journeyCreate);
  crow::json::rvalue parsed = crow::json::load(response.text);

  ASSERT_TRUE(parsed);
  JourneyResponse jResponse = api::json::fromJson<JourneyResponse>(parsed);
  EXPECT_EQ(jResponse.name, journeyCreate.name);
}
