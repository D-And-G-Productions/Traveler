#include "domain/Journey.h"
#include "repository/JourneyRepository.h"
#include "support/CrowRouteFixture.h"
#include "json/JourneyJson.h"
#include <crow/common.h>
#include <crow/http_request.h>
#include <crow/http_response.h>
#include <crow/json.h>
#include <gtest/gtest.h>
#include <iostream>
#include <ostream>

class JourneyPostFixture : public CrowRouteFixture {
public:
  crow::response handleJourneyPost(const Journey &journey) {
    crow::request request;
    request.url = "/journey";
    request.method = crow::HTTPMethod::POST;
    request.body = JourneyJSON::toJson(journey).dump();

    crow::response response;
    server->app().handle_full(request, response);
    return response;
  }
};

TEST_F(JourneyPostFixture, PostReturnsJourneyWithId) {
  Journey journey{};
  journey.name = "NAME";

  std::cout << JourneyJSON::toJson(journey).dump() << std::endl;
  crow::response response = handleJourneyPost(journey);

  crow::json::rvalue parsed = crow::json::load(response.body);
  JourneyRecord receivedJourneyRecord = JourneyJSON::toJourneyRecord(parsed);
  ASSERT_EQ(receivedJourneyRecord.value.name, "NAME");
}
