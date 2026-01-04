#include "cpr/status_codes.h"
#include "domain/Journey.hpp"
#include "domain/JourneyCreate.hpp"
#include "support/fixtures/JourneyIdDelete.hpp"
#include "support/fixtures/RouteFixture.hpp"
#include <crow/common.h>
#include <crow/http_response.h>
#include <crow/json.h>
#include <cstdint>
#include <gtest/gtest.h>

TEST_F(JourneyIdDelete, DeletingNonExistantJourneyReturnsBadRequest) {
  const int64_t NON_EXISTENT_ID = 0;
  cpr::Response response = journeyIdDelete(testUser.token, NON_EXISTENT_ID);
  ASSERT_EQ(response.status_code, cpr::status::HTTP_NOT_FOUND);
}

TEST_F(JourneyIdDelete, DeletingExistingJourneyWithDifferentID) {
  TestUser diff = createTestUser("bob");
  JourneyCreate journeyCreate{.userId = diff.user.id, .name = "bob"};
  Journey journey = createJourney(journeyCreate);
  cpr::Response response = journeyIdDelete(testUser.token, journey.id);
  ASSERT_EQ(response.status_code, cpr::status::HTTP_NOT_FOUND);
}

TEST_F(JourneyIdDelete, DeletingExistingJourney) {
  JourneyCreate journeyCreate{.userId = stoi(testUser.sub), .name = "TEST_NAME"};
  Journey journey = createJourney(journeyCreate);

  cpr::Response response = journeyIdDelete(testUser.token, journey.id);
  ASSERT_EQ(response.status_code, cpr::status::HTTP_NO_CONTENT);
}

TEST_F(JourneyIdDelete, DeletingExistingJourneyIsReflectedInDatabase) {
  JourneyCreate journeyCreate{.userId = stoi(testUser.sub), .name = "TEST_NAME"};
  Journey journey = createJourney(journeyCreate);

  journeyIdDelete(testUser.token, journey.id);
  cpr::Response response = journeyIdDelete(testUser.token, journey.id);
  ASSERT_EQ(response.status_code, cpr::status::HTTP_NOT_FOUND);
}
