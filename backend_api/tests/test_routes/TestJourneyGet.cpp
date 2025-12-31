#include "cpr/response.h"
#include "cpr/status_codes.h"
#include "domain/JourneyCreate.hpp"
#include "support/fixtures/JourneyGet.hpp"
#include <crow/common.h>
#include <crow/http_response.h>
#include <crow/json.h>
#include <gtest/gtest.h>
#include <string>

using std::string;

TEST_F(JourneyGet, ReturnsOK) {
  cpr::Response response = journeyGet();
  EXPECT_EQ(response.status_code, cpr::status::HTTP_OK);
}

TEST_F(JourneyGet, ReceiveEmptyJourneys) {
  cpr::Response response = journeyGet();

  crow::json::rvalue parsed = crow::json::load(response.text);
  ASSERT_TRUE(parsed);
  EXPECT_EQ(parsed.t(), crow::json::type::List);
  EXPECT_EQ(parsed.size(), 0);
}

TEST_F(JourneyGet, ReceiveOneJourney) {
  JourneyCreate journeyCreate{};
  server->journeyRepository->create(journeyCreate);

  cpr::Response response = journeyGet();

  crow::json::rvalue parsed = crow::json::load(response.text);
  ASSERT_TRUE(parsed);
  EXPECT_EQ(parsed.t(), crow::json::type::List);
  EXPECT_EQ(parsed.size(), 1);
}

TEST_F(JourneyGet, ReceiveTwoJourneys) {
  JourneyCreate journeyCreate{};
  server->journeyRepository->create(journeyCreate);
  server->journeyRepository->create(journeyCreate);

  cpr::Response response = journeyGet();

  crow::json::rvalue parsed = crow::json::load(response.text);
  ASSERT_TRUE(parsed);
  EXPECT_EQ(parsed.t(), crow::json::type::List);
  EXPECT_EQ(parsed.size(), 2);
}
