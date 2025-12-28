#include "domain/Journey.h"
#include "repository/JourneyRepository.h"
#include "support/CrowRouteFixture.h"
#include <crow/common.h>
#include <crow/http_response.h>
#include <crow/json.h>
#include <cstdint>
#include <gtest/gtest.h>

class JourneyIdDeleteFixture : public CrowRouteFixture {
public:
  crow::response handlePutJourney(const int64_t id, const Journey &journey) {
    crow::request request;
    request.method = crow::HTTPMethod::DELETE;
    request.url = "/journey/" + std::to_string(id);

    crow::response response;
    server->app().handle_full(request, response);
    return response;
  }
};

TEST_F(JourneyIdDeleteFixture, DeletingNonExistantJourneyReturnsBadRequest) {
  const int64_t NON_EXISTENT_ID = 0;
  const Journey journey{};
  crow::response response = handlePutJourney(NON_EXISTENT_ID, journey);
  ASSERT_EQ(response.code, crow::NOT_FOUND);
}

TEST_F(JourneyIdDeleteFixture, DeletingExistingJourneyIsReflectedInDatabase) {
  std::string ORIGINAL_NAME = "ORIGINAL";
  Journey journey{};
  journey.name = ORIGINAL_NAME;
  JourneyRecord originalJourneyRecord = server->repository()->create(journey);

  handlePutJourney(originalJourneyRecord.id, journey);
  JourneyRecord updatedJourneyRecord = server->repository()->get(originalJourneyRecord.id);
  ASSERT_EQ(updatedJourneyRecord, NULL);
}
