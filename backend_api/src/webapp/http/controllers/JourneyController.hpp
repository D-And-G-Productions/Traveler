#pragma once

#include "api/Mappings.hpp"
#include "api/dto/JourneyCreateRequest.hpp"
#include "api/dto/JourneyResponse.hpp"
#include "api/json/JsonConversion.hpp"
#include "domain/JourneyCreate.hpp"
#include "http/middleware/AuthMiddleware.hpp"
#include "repository/JourneyRepository.hpp"
#include "repository/UserRepository.hpp"
#include <crow/app.h>
#include <crow/common.h>
#include <crow/http_request.h>
#include <crow/http_response.h>
#include <crow/json.h>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <vector>

using context = AuthMiddleware::context;
using crow::request;
using crow::response;
using crow::HTTPMethod::DELETE;
using crow::HTTPMethod::GET;
using crow::HTTPMethod::POST;
using crow::HTTPMethod::PUT;
using std::shared_ptr;

class JourneyController {
private:
  shared_ptr<JourneyRepository> journeyRepo;
  shared_ptr<UserRepository> userRepo;

public:
  JourneyController(
      shared_ptr<JourneyRepository> journeyRepo_, shared_ptr<UserRepository> userRepo_
  )
      : journeyRepo(journeyRepo_), userRepo(userRepo_) {}

  void registerRoutes(crow::App<AuthMiddleware> &app) {
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
        .methods(DELETE)([this, &app](const crow::request &request, const long id) {
          context &context = app.get_context<AuthMiddleware>(request);
          return deleteJourney(context, request, static_cast<int64_t>(id));
        });

    CROW_ROUTE(app, "/journey/<int>")
        .methods(PUT)([this, &app](const crow::request &request, long id) {
          context &context = app.get_context<AuthMiddleware>(request);
          return updateJourney(context, request, static_cast<int64_t>(id));
        });
  }

private:
  struct FailedToReadJson : public std::runtime_error {
    using std::runtime_error::runtime_error;
  };

  struct ConversionFromJsonFailed : public std::runtime_error {
    using std::runtime_error::runtime_error;
  };

  crow::json::rvalue parseJsonBody(const std::string body) {
    crow::json::rvalue json = crow::json::load(body);
    if (!json)
      throw FailedToReadJson("Invalid Json");
    return json;
  }

  JourneyCreateRequest jsonToJourneyCreateRequest(const crow::json::rvalue json) {
    try {
      JourneyCreateRequest journeyCreateRequest = api::json::fromJson<JourneyCreateRequest>(json);
      return journeyCreateRequest;
    } catch (std::exception &error) {
      throw ConversionFromJsonFailed("Conversion from JSON failed.");
    }
  }

  crow::response createJourney(const context &context, const request &request) {
    crow::response response;
    try {
      crow::json::rvalue json = parseJsonBody(request.body);
      JourneyCreateRequest journeyCreateRequest = jsonToJourneyCreateRequest(json);
      JourneyCreate journeyCreate = toJourneyCreate(context.userId, journeyCreateRequest);
      Journey journey = journeyRepo->insert(journeyCreate);
      response = buildResponseWithJourneyBody(crow::CREATED, journey);
    } catch (FailedToReadJson &) {
      response = crow::response(crow::status::BAD_REQUEST, "Invalid JSON.");
    } catch (ConversionFromJsonFailed &) {
      response = crow::response(crow::BAD_REQUEST, "Missing/invalid fields");
    }
    return response;
  }

  crow::json::wvalue toJsonArray(const std::vector<Journey> &journeys) {
    crow::json::wvalue::list journeysJson;
    journeysJson.resize(journeys.size());

    for (size_t i{0}; i < journeys.size(); i++) {
      JourneyResponse journeyResponse = toJourneyResponse(journeys[i]);
      journeysJson[i] = api::json::toJson(journeyResponse);
    }

    return journeysJson;
  }

  response listJourneys(const context &context, const request &request) {
    std::vector<Journey> journeys = journeyRepo->selectByUserId(context.userId);
    crow::json::wvalue json = toJsonArray(journeys);
    return crow::response{crow::OK, "application/json", json.dump()};
  }

  response getJourney(const context &context, const request &request, const int64_t id) {
    Journey journey;

    try {
      journey = journeyRepo->selectById(id);
    } catch (std::invalid_argument &error) {
      return crow::response{crow::NOT_FOUND, "Not Found"};
    }

    return buildResponseWithJourneyBody(crow::OK, journey);
  }

  response deleteJourney(const context &context, const request &request, const int64_t id) {
    Journey journey;
    try {

      journey = journeyRepo->selectById(id);
      if (journey.userId == context.userId) {
        journeyRepo->del(id);
      } else {
        return crow::response(crow::NOT_FOUND, "Not Found");
      }
    } catch (const std::invalid_argument &error) {
      return crow::response(crow::NOT_FOUND, "Not Found");
    }
    return crow::response(crow::NO_CONTENT, "No Content");
  }

  response buildResponseWithJourneyBody(const crow::status code, const Journey &journey) {
    JourneyResponse journeyResponse = toJourneyResponse(journey);
    crow::json::wvalue json = api::json::toJson(journeyResponse);
    return crow::response{code, "application/json", json.dump()};
  }

  response updateJourney(const context &context, const request &request, const int64_t id) {
    crow::json::rvalue body = crow::json::load(request.body);
    if (!body) {
      return crow::response(crow::status::BAD_REQUEST, "Invalid JSON");
    }
    JourneyCreateRequest journeyCreateRequest = api::json::fromJson<JourneyCreateRequest>(body);
    // TODO: Change this to use the context to select the user id. 2025-12-30
    JourneyCreate journeyCreate = toJourneyCreate(0, journeyCreateRequest);
    Journey journey = toJourney(id, journeyCreate);

    Journey updatedJourney;
    try {
      updatedJourney = journeyRepo->update(id, journeyCreate);
    } catch (std::invalid_argument &error) {
      return crow::response(crow::NOT_FOUND);
    }
    return crow::response(crow::NO_CONTENT);
  }
};
