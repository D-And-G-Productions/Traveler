#include "api/dto/JourneyCreateRequest.hpp"
#include "cpr/response.h"
#include "cpr/status_codes.h"
#include "domain/JourneyCreate.hpp"
#include "support/fixtures/JourneyIdPut.hpp"
#include <crow/common.h>
#include <crow/http_response.h>
#include <crow/json.h>
#include <cstdint>
#include <gtest/gtest.h>

using std::string;

TEST_F(JourneyIdPut, UpdateExistingRecordReturnsNoContent) {
  const int64_t NON_EXISTENT_ID = 0;
  const JourneyCreate journeyCreate{};
  const Journey existingJourney = testServer->journeyRepo->insert(journeyCreate);
  const JourneyCreateRequest jcRequest{};
  const cpr::Response response = journeyIdPut("TOKEN", NON_EXISTENT_ID, jcRequest);
  ASSERT_EQ(response.status_code, cpr::status::HTTP_NO_CONTENT);
}

TEST_F(JourneyIdPut, NonExistantIdReturnsNotFound) {
  const int64_t NON_EXISTENT_ID = 0;
  const JourneyCreateRequest jcRequest{};
  const cpr::Response response = journeyIdPut("TOKEN", NON_EXISTENT_ID, jcRequest);
  ASSERT_EQ(response.status_code, cpr::status::HTTP_NOT_FOUND);
}

TEST_F(JourneyIdPut, UpdateIsReflectedInDatabase) {
  const string ORIGINAL_NAME = "ORIGINAL";
  const string UPDATED_NAME = "UPDATED";

  const JourneyCreate journeyCreate{.name = ORIGINAL_NAME};
  const Journey originalJourney = testServer->journeyRepo->insert(journeyCreate);

  const JourneyCreateRequest jcRequest{.name = UPDATED_NAME};
  journeyIdPut("TOKEN", originalJourney.id, jcRequest);

  const Journey updatedJourney = testServer->journeyRepo->selectById(originalJourney.id);
  ASSERT_EQ(updatedJourney.name, UPDATED_NAME);
}
