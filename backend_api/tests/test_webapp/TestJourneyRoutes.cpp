#include "api/dto/JourneyCreateRequestContract.hpp"
#include "api/dto/JourneyResponseContract.hpp"
#include "api/dto/JourneyUpdateRequest.hpp"
#include "api/json/JsonConversion.hpp"
#include "cpr/status_codes.h"
#include "fixtures/JourneyRoutes.hpp"
#include "persistence/StoreErrors.hpp"
#include <crow/common.h>
#include <gtest/gtest.h>

namespace
{
JourneyCreateRequestContract placeHolderJourneyCreateRequest()
{
  return {
      .arrivalTime = "2026-01-09 12:00:00+00",
      .mode = Mode::TRANSIT,
  };
}
} // namespace

// TODO: Add tests that check for unauthorized tokens being bounced, this will require changing the
// mock token authorizer.

TEST_F(JourneyRoutes, GetJourneysReturnsOK)
{
  cpr::Response response = journeysGet(testUser.token);
  EXPECT_EQ(response.status_code, cpr::status::HTTP_OK);
}

TEST_F(JourneyRoutes, GetJourneysInvalidTokenReturnsUnauthorized)
{
  cpr::Response response = journeysGet("INVALID TOKEN");
  EXPECT_EQ(response.status_code, cpr::status::HTTP_UNAUTHORIZED);
}

TEST_F(JourneyRoutes, GetJourneysUserReceivesZeroJourneys)
{
  cpr::Response response = journeysGet(testUser.token);
  crow::json::rvalue journeysJson = crow::json::load(response.text);
  EXPECT_EQ(journeysJson.size(), 0);
}

TEST_F(JourneyRoutes, GetJourneysUserReceivesOneJourney)
{
  createJourneyForUser(testUser.user.id);
  cpr::Response response = journeysGet(testUser.token);
  crow::json::rvalue journeysJson = crow::json::load(response.text);

  EXPECT_EQ(journeysJson.size(), 1);
}

TEST_F(JourneyRoutes, GetJourneysUserReceivesTwoJourneys)
{
  JourneyCreate journeyCreate{.userId = testUser.user.id};
  createJourneyForUser(testUser.user.id);
  createJourneyForUser(testUser.user.id);
  cpr::Response response = journeysGet(testUser.token);
  crow::json::rvalue journeysJson = crow::json::load(response.text);

  EXPECT_EQ(journeysJson.size(), 2);
}

TEST_F(JourneyRoutes, GetJourneysUserReceivesOnlyTheirJourneys)
{
  TestUser otherUser = createTestUser();
  createJourneyForUser(otherUser.user.id);
  cpr::Response response = journeysGet(testUser.token);
  crow::json::rvalue json = crow::json::load(response.text);

  EXPECT_EQ(json.size(), 0);
}

TEST_F(JourneyRoutes, GetJourneyByIdExistingIdReturnsOk)
{
  Journey journey = createJourneyForUser(testUser.user.id);
  cpr::Response response = journeyIdGet(testUser.token, journey.id);
  EXPECT_EQ(response.status_code, cpr::status::HTTP_OK) << response.text << "\n";
}

TEST_F(JourneyRoutes, GetJourneyByIdNonExistentIdReturnsNotFound)
{
  const int64_t NON_EXISTENT_ID = 0;
  cpr::Response response = journeyIdGet(testUser.token, NON_EXISTENT_ID);
  EXPECT_EQ(response.status_code, crow::NOT_FOUND) << response.text << "\n";
}

TEST_F(JourneyRoutes, GetJourneyByIdResponseBodyIsParsed)
{
  Journey journey = createJourneyForUser(testUser.user.id);
  cpr::Response response = journeyIdGet(testUser.token, journey.id);
  crow::json::rvalue json = crow::json::load(response.text);

  ASSERT_TRUE(json);
}

TEST_F(JourneyRoutes, GetJourneyByIdReturnJourneyWithMatchingInformation)
{
  Journey journey = createJourneyForUser(testUser.user.id);
  cpr::Response response = journeyIdGet(testUser.token, journey.id);
  crow::json::rvalue json = crow::json::load(response.text);
  JourneyResponseContract received = JsonConversion::fromJson<JourneyResponseContract>(json);

  EXPECT_EQ(received.id, journey.id);
}

TEST_F(JourneyRoutes, GetJourneyByIdUsersCannotSeeOtherUserJourneys)
{
  TestUser otherUser = createTestUser();
  Journey journey = createJourneyForUser(otherUser.user.id);
  cpr::Response response = journeyIdGet(testUser.token, journey.id);

  ASSERT_EQ(response.status_code, cpr::status::HTTP_NOT_FOUND);
}

TEST_F(JourneyRoutes, PostReturnsCreated)
{
  JourneyCreateRequestContract jcr = placeHolderJourneyCreateRequest();
  cpr::Response response = journeyPost(testUser.token, jcr);

  EXPECT_EQ(response.status_code, cpr::status::HTTP_CREATED) << response.text << "\n";
}

TEST_F(JourneyRoutes, PostBodyIsJson)
{
  JourneyCreateRequestContract jcr = placeHolderJourneyCreateRequest();
  cpr::Response response = journeyPost(testUser.token, jcr);
  crow::json::rvalue parsed = crow::json::load(response.text);

  ASSERT_TRUE(parsed) << response.text << "\n";
}

TEST_F(JourneyRoutes, PostIsReflectedInDB)
{
  JourneyCreateRequestContract jcr = placeHolderJourneyCreateRequest();
  cpr::Response response = journeyPost(testUser.token, jcr);
  crow::json::rvalue json = crow::json::load(response.text);
  JourneyResponseContract journeyResponse = JsonConversion::fromJson<JourneyResponseContract>(json);
  Journey journeyFromDB = getJourneyFromDB(journeyResponse.id);

  EXPECT_EQ(journeyFromDB.id, journeyResponse.id);
}

TEST_F(JourneyRoutes, DeletingNonExistantJourneyReturnsNotFound)
{
  const int64_t NON_EXISTENT_ID = 0;
  cpr::Response response = journeyIdDelete(testUser.token, NON_EXISTENT_ID);
  ASSERT_EQ(response.status_code, cpr::status::HTTP_NOT_FOUND);
}

TEST_F(JourneyRoutes, DeletingJourneyOfOtherUserReturnsNotFound)
{
  TestUser otherUser = createTestUser();
  Journey journey = createJourneyForUser(otherUser.user.id);
  cpr::Response response = journeyIdDelete(testUser.token, journey.id);
  ASSERT_EQ(response.status_code, cpr::status::HTTP_NOT_FOUND);
}

TEST_F(JourneyRoutes, DeletingExistingJourneyReturnsNotContent)
{
  Journey journey = createJourneyForUser(testUser.user.id);
  cpr::Response response = journeyIdDelete(testUser.token, journey.id);
  ASSERT_EQ(response.status_code, cpr::status::HTTP_NO_CONTENT);
}

TEST_F(JourneyRoutes, DeletingExistingJourneyIsReflectedInDatabase)
{
  Journey journey = createJourneyForUser(testUser.user.id);
  journeyIdDelete(testUser.token, journey.id);
  cpr::Response response = journeyIdDelete(testUser.token, journey.id);
  ASSERT_THROW({ getJourneyFromDB(journey.id); }, JourneyNotFoundError);
}

TEST_F(JourneyRoutes, JourneyIdPutUpdateJourneyReturnsNoContent)
{
  Journey journey = createJourneyForUser(testUser.user.id);
  const JourneyUpdateRequestContract updateContract{};
  const cpr::Response response = journeyIdPut(testUser.token, journey.id, updateContract);

  ASSERT_EQ(response.status_code, cpr::status::HTTP_NO_CONTENT) << response.text << "\n";
}

TEST_F(JourneyRoutes, JourneyIdPutUpdateNonExistantJourneyReturnsNotFound)
{
  const int64_t NON_EXISTENT_ID = 0;
  const JourneyUpdateRequestContract updateContract{};
  const cpr::Response response = journeyIdPut(testUser.token, NON_EXISTENT_ID, updateContract);

  ASSERT_EQ(response.status_code, cpr::status::HTTP_NOT_FOUND);
}

TEST_F(JourneyRoutes, JourneyIdPutVerifyNameIsUpdated)
{
  std::string UPDATED_ARRIVAL_TIME = "3000-01-09 12:00:00+00";
  const Journey journey = createJourneyForUser(testUser.user.id);
  JourneyUpdateRequestContract updateContract{.arrivalTime = UPDATED_ARRIVAL_TIME};
  cpr::Response response = journeyIdPut(testUser.token, journey.id, updateContract);

  ASSERT_EQ(response.status_code, cpr::status::HTTP_NO_CONTENT);

  const Journey updatedJourney = getJourneyFromDB(journey.id);
  ASSERT_EQ(updatedJourney.arrivalTime, UPDATED_ARRIVAL_TIME);
}

TEST_F(JourneyRoutes, ExistingIdReturnsOk)
{
  Journey journey = createJourneyForUser(testUser.user.id);
  cpr::Response response = journeyIdGet(testUser.token, journey.id);
  EXPECT_EQ(response.status_code, cpr::status::HTTP_OK) << response.text << "\n";
}

TEST_F(JourneyRoutes, NonExistentIdReturnsNotFound)
{
  const int64_t NON_EXISTENT_ID = 0;
  cpr::Response response = journeyIdGet(testUser.token, NON_EXISTENT_ID);

  EXPECT_EQ(response.status_code, crow::NOT_FOUND) << response.text << "\n";
}

TEST_F(JourneyRoutes, ResponseBodyIsParsed)
{
  Journey journey = createJourneyForUser(testUser.user.id);
  cpr::Response response = journeyIdGet(testUser.token, journey.id);
  crow::json::rvalue json = crow::json::load(response.text);

  EXPECT_TRUE(json);
}

TEST_F(JourneyRoutes, ReturnJourneyWithMatchingInformation)
{
  Journey journey = createJourneyForUser(testUser.user.id);
  cpr::Response response = journeyIdGet(testUser.token, journey.id);
  crow::json::rvalue json = crow::json::load(response.text);
  JourneyResponseContract jrc = JsonConversion::fromJson<JourneyResponseContract>(json);

  EXPECT_EQ(jrc.id, journey.id);
  EXPECT_EQ(jrc.arrivalTime, journey.arrivalTime);
}
