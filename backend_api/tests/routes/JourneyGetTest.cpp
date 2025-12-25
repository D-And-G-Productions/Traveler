#include "domain/Journey.h"
#include "repository/JourneyRepository.h"
#include "support/CrowRouteFixture.h"
#include "json/JourneyJson.h"
#include <crow/common.h>
#include <crow/http_response.h>
#include <crow/json.h>
#include <gtest/gtest.h>

class JourneyIdGetFixture : public CrowRouteFixture {
public:
  std::string journeyUrl() { return server->baseUrl() + "/journey"; }

  crow::response handleGetJourney() {
    crow::request request;
    request.method = crow::HTTPMethod::Get;
    request.url = "/journey";

    crow::response response;
    server->app().handle_full(request, response);
    return response;
  }

  crow::response handlePutJourney(const int64_t id) {
    crow::request request;
    request.method = crow::HTTPMethod::Get;
    request.url = "/journey/" + std::to_string(id);

    crow::response response;
    server->app().handle_full(request, response);
    return response;
  }
};

TEST_F(JourneyIdGetFixture, GetEmptyReturnsJsonArray) {
  crow::response response = handleGetJourney();
  EXPECT_EQ(response.code, crow::OK);

  crow::json::rvalue parsed = crow::json::load(response.body);
  ASSERT_TRUE(parsed);
  ASSERT_EQ(parsed.t(), crow::json::type::List);
  EXPECT_EQ(parsed.size(), 0);
}

TEST_F(JourneyIdGetFixture, GetNonEmptyReturnsJsonArray) {
  Journey journey{};
  server->repository()->create(std::move(journey));

  crow::response response = handleGetJourney();
  EXPECT_EQ(response.code, crow::OK);

  crow::json::rvalue parsed = crow::json::load(response.body);
  ASSERT_TRUE(parsed);
  ASSERT_EQ(parsed.t(), crow::json::type::List);
  EXPECT_EQ(parsed.size(), 1);
}

TEST_F(JourneyIdGetFixture, GetReturnsJourneyWithId) {
  Journey journey{};
  JourneyRecord expectedJourneyRecord = server->repository()->create(journey);

  crow::response response = handleGetJourney();
  EXPECT_EQ(response.code, crow::OK);

  crow::json::rvalue parsed = crow::json::load(response.body);
  JourneyRecord receivedJourneyRecord = JourneyJSON::toJourneyRecord(parsed[0]);
  ASSERT_EQ(expectedJourneyRecord.id, receivedJourneyRecord.id);
}
