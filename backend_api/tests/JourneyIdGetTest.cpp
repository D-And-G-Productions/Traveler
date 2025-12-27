#include "domain/Journey.h"
#include "repository/JourneyRepository.h"
#include "support/CrowRouteFixture.h"
#include "json/JourneyJson.h"
#include <crow/common.h>
#include <crow/http_response.h>
#include <crow/json.h>
#include <cstdint>
#include <gtest/gtest.h>

class JourneyIdGetFixture : public CrowRouteFixture {
public:
  std::string journeyUrl() { return server->baseUrl() + "/journey"; }

  crow::response handleGetJourney(const int64_t id) {
    crow::request request;
    request.method = crow::HTTPMethod::Get;
    request.url = "/journey/" + std::to_string(id);

    crow::response response;
    server->app().handle_full(request, response);
    return response;
  }
};

TEST_F(JourneyIdGetFixture, ReturnJourneyWithMatchingId) {
  Journey journey{};
  JourneyRecord expectedJourneyRecord = server->repository()->create(journey);

  crow::response response = handleGetJourney(expectedJourneyRecord.id);
  EXPECT_EQ(response.code, crow::OK);

  crow::json::rvalue parsed = crow::json::load(response.body);
  JourneyRecord receivedJourneyRecord = JourneyJSON::toJourneyRecord(parsed);
  ASSERT_EQ(expectedJourneyRecord.id, receivedJourneyRecord.id);
}

TEST_F(JourneyIdGetFixture, ReturnNotFoundOnNonExistentId) {
  const int64_t NON_EXISTENT_ID = 0;
  crow::response response = handleGetJourney(NON_EXISTENT_ID);
  EXPECT_EQ(response.code, crow::NOT_FOUND);
}
