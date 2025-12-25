#include "domain/Journey.h"
#include "repository/JourneyRepository.h"
#include "support/CrowRouteFixture.h"
#include "json/JourneyJson.h"
#include <crow/common.h>
#include <crow/http_response.h>
#include <crow/json.h>
#include <cstdint>
#include <gtest/gtest.h>
#include <string>

class JourneyIdPutFixture : public CrowRouteFixture {
public:
  crow::response handlePutJourney(const int64_t id, const Journey &journey) {
    crow::request request;
    request.url = "/journey/" + std::to_string(id);
    request.method = crow::HTTPMethod::PUT;
    request.body = JourneyJSON::toJson(journey).dump();

    crow::response response;
    server->app().handle_full(request, response);
    return response;
  }
};

TEST_F(JourneyIdPutFixture, UpdatingNonExistantJourneyReturnsBadRequest) {
  const int64_t NON_EXISTENT_ID = 0;
  const Journey journey{};
  crow::response response = handlePutJourney(NON_EXISTENT_ID, journey);
  ASSERT_EQ(response.code, crow::NOT_FOUND);
}

TEST_F(JourneyIdPutFixture, UpdateExistingRecordReturnsNoContent) {
  Journey journey{};
  journey.name = std::string("TEST_JOURNEY");
  JourneyRecord originalJourneyRecord = server->repository()->create(journey);

  crow::response response = handlePutJourney(originalJourneyRecord.id, journey);
  ASSERT_EQ(response.code, crow::NO_CONTENT);
}

TEST_F(JourneyIdPutFixture, UpdatedExistingJourneyIsReflectedInDatabase) {
  std::string ORIGINAL_NAME = "ORIGINAL", UPDATED_NAME = "UPDATED";
  Journey journey{};
  journey.name = ORIGINAL_NAME;
  JourneyRecord originalJourneyRecord = server->repository()->create(journey);

  journey.name = UPDATED_NAME;
  handlePutJourney(originalJourneyRecord.id, journey);
  JourneyRecord updatedJourneyRecord = server->repository()->get(originalJourneyRecord.id);
  Journey updateJourney = updatedJourneyRecord.value;
  ASSERT_EQ(updateJourney.name, UPDATED_NAME);
}
