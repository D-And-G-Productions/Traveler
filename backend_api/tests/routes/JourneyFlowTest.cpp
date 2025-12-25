#include "support/CPRRouteFixture.h"
#include "support/JourneyTestData.h"
#include <crow/json.h>
#include <string>

class JourneyFlowFixture : public CPRRouteFixture {
public:
  std::string journeyUrl() { return server->baseUrl() + "/journey"; }
};

TEST_F(JourneyFlowFixture, PostThenGetIncludesJourney) {
  crow::json::wvalue expectedJSONWrite = makeJourneyJson();
  crow::json::rvalue expected = crow::json::load(expectedJSONWrite.dump());

  cpr::Response postResp = postJson(journeyUrl(), expectedJSONWrite.dump());
  EXPECT_EQ(postResp.status_code, 200);

  cpr::Response getResp = get(journeyUrl());
  EXPECT_EQ(getResp.status_code, 200);

  crow::json::rvalue parsed = crow::json::load(getResp.text);
  ASSERT_TRUE(parsed);
  ASSERT_EQ(parsed.t(), crow::json::type::List);
  ASSERT_EQ(parsed.size(), 1);

  const crow::json::rvalue &got = parsed[0];
  EXPECT_EQ(std::string(got["name"].s()), std::string(expected["name"].s()));
}
