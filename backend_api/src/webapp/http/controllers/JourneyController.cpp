#include "JourneyController.hpp"
#include "api/Mappings.hpp"
#include "api/json/JsonConversion.hpp"

void JourneyController::registerRoutes(crow::App<AuthMiddleware> &app) {
  CROW_ROUTE(app, "/journey").methods(POST)([this, &app](const crow::request &request) {
    context &context = app.get_context<AuthMiddleware>(request);
    return createJourney(context, request);
  });

  CROW_ROUTE(app, "/journey").methods(GET)([this, &app](const crow::request &request) {
    context &context = app.get_context<AuthMiddleware>(request);
    return listJourneys(context, request);
  });

  CROW_ROUTE(app, "/journey/<int>")
      .methods(GET)([this, &app](const crow::request &request, const long id) {
        context &context = app.get_context<AuthMiddleware>(request);
        return getJourney(context, request, static_cast<int64_t>(id));
      });

  CROW_ROUTE(app, "/journey/<int>")
      .methods(PUT)([this, &app](const crow::request &request, long id) {
        context &context = app.get_context<AuthMiddleware>(request);
        return updateJourney(context, request, static_cast<int64_t>(id));
      });
}

crow::response
JourneyController::createJourney(const context &context, const crow::request &request) {
  crow::response response;
  try {
    crow::json::rvalue json = parseJson(request.body);
    JourneyCreateRequest journeyCreateRequest = jsonToJourneyCreateRequest(json);
    JourneyCreate journeyCreate = toJourneyCreate(context.userId, journeyCreateRequest);
    Journey journey = journeyService->createJourney(journeyCreate);
    response = buildResponseWithJourneyBody(crow::CREATED, journey);
  } catch (FailedToReadJson &) {
    response = crow::response(crow::status::BAD_REQUEST, "Invalid JSON");
  } catch (ConversionFromJsonFailed &) {
    response = crow::response(crow::BAD_REQUEST, "Missing/invalid fields");
  }
  return response;
}

crow::response JourneyController::updateJourney(
    const context &context,
    const crow::request &request,
    const int64_t id
) {
  crow::response response;
  try {
    crow::json::rvalue json = parseJson(request.body);
    JourneyCreateRequest journeyCreateRequest = api::json::fromJson<JourneyCreateRequest>(json);
    JourneyCreate journeyCreate = toJourneyCreate(context.userId, journeyCreateRequest);
    journeyService->updateJourney(context.userId, id, journeyCreate);
    response = crow::response(crow::NO_CONTENT);
  } catch (FailedToReadJson &) {
    response = crow::response(crow::status::BAD_REQUEST, "Invalid JSON");
  } catch (ConversionFromJsonFailed &) {
    response = crow::response(crow::BAD_REQUEST, "Missing/invalid fields");
  } catch (JourneyNotFoundError &) {
    response = crow::response{crow::NOT_FOUND, "Not Found"};
  }
  return response;
}

crow::json::rvalue JourneyController::parseJson(const std::string body) {
  crow::json::rvalue json = crow::json::load(body);
  if (!json)
    throw FailedToReadJson("Invalid Json");
  return json;
}

JourneyCreateRequest JourneyController::jsonToJourneyCreateRequest(const crow::json::rvalue json) {
  try {
    JourneyCreateRequest journeyCreateRequest = api::json::fromJson<JourneyCreateRequest>(json);
    return journeyCreateRequest;
  } catch (std::exception &error) {
    throw ConversionFromJsonFailed("Conversion from JSON failed.");
  }
}

crow::response JourneyController::getJourney(
    const context &context,
    const crow::request &request,
    const int64_t id
) {
  Journey journey;
  try {
    journey = journeyService->getJourney(context.userId, id);
    return buildResponseWithJourneyBody(crow::OK, journey);
  } catch (JourneyNotFoundError &) {
    return crow::response{crow::NOT_FOUND, "Not Found"};
  }
}

crow::response
JourneyController::buildResponseWithJourneyBody(const crow::status code, const Journey &journey) {
  JourneyResponse journeyResponse = toJourneyResponse(journey);
  crow::json::wvalue json = api::json::toJson(journeyResponse);
  return crow::response{code, "application/json", json.dump()};
}

crow::response
JourneyController::listJourneys(const context &context, const crow::request &request) {
  std::vector<Journey> journeys = journeyService->getJourneys(context.userId);
  crow::json::wvalue json = toJsonArray(journeys);
  return crow::response{crow::OK, "application/json", json.dump()};
}

crow::json::wvalue JourneyController::toJsonArray(const std::vector<Journey> &journeys) {
  crow::json::wvalue::list journeysJson;
  journeysJson.resize(journeys.size());

  for (size_t i{0}; i < journeys.size(); i++) {
    JourneyResponse journeyResponse = toJourneyResponse(journeys[i]);
    journeysJson[i] = api::json::toJson(journeyResponse);
  }

  return journeysJson;
}
