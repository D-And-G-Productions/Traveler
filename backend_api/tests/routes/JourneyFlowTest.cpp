#include "domain/Journey.h"
#include "support/CPRRouteFixture.h"
#include "json/JourneyJson.h"
#include <crow/json.h>
#include <gtest/gtest.h>
#include <string>

class JourneyFlowFixture : public CPRRouteFixture {
public:
  std::string journeyUrl() { return server->baseUrl() + "/journey"; }
};

TEST_F(JourneyFlowFixture, PostThenGetIncludesJourney) {
  Journey journey{};
  journey.name = "TEST_NAME";
  crow::json::wvalue body = JourneyJSON::toJson(journey);

  cpr::Response postResp = post(journeyUrl(), body.dump());
  ASSERT_EQ(postResp.status_code, 200) << postResp.text << "\n";

  cpr::Response getResp = get(journeyUrl());
  ASSERT_EQ(getResp.status_code, 200) << postResp.text << "\n";
  crow::json::rvalue parsed = crow::json::load(getResp.text);
  ASSERT_TRUE(parsed);
  ASSERT_EQ(parsed.t(), crow::json::type::List);
  ASSERT_EQ(parsed.size(), 1);

  const crow::json::rvalue &receivedBody = parsed[0];
  EXPECT_EQ(std::string(receivedBody["name"].s()), journey.name);
}
