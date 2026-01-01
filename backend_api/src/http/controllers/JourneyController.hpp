#pragma once

#include "api/Mappings.hpp"
#include "api/dto/JourneyCreateRequest.hpp"
#include "api/dto/JourneyResponse.hpp"
#include "api/json/JsonConversion.hpp"
#include "domain/JourneyCreate.hpp"
#include "http/middleware/AuthMiddleware.hpp"
#include "repository/JourneyRepository.hpp"
#include <crow/app.h>
#include <crow/common.h>
#include <crow/http_request.h>
#include <crow/http_response.h>
#include <cstdint>
#include <memory>
#include <stdexcept>

using context = AuthMiddleware::context;
using crow::request;
using crow::response;
using crow::HTTPMethod::GET;
using crow::HTTPMethod::POST;
using crow::HTTPMethod::PUT;

class JourneyController {
public:
  JourneyController(std::shared_ptr<JourneyRepository> repository) : repository_(repository) {}

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
        .methods(PUT)([this, &app](const crow::request &request, long id) {
          context &context = app.get_context<AuthMiddleware>(request);
          return updateJourney(context, request, static_cast<int64_t>(id));
        });
  }

private:
  std::shared_ptr<JourneyRepository> repository_;

  response createJourney(const context &context, const request &request) {
    crow::json::rvalue body = crow::json::load(request.body);
    if (!body) {
      return crow::response(crow::status::BAD_REQUEST, "Invalid JSON");
    }

    JourneyCreateRequest journeyCreateRequest;
    try {
      journeyCreateRequest = api::json::fromJson<JourneyCreateRequest>(body);
    } catch (std::exception &error) {
      return crow::response(crow::BAD_REQUEST, "Missing/invalid fields");
    }

    // TODO: Change this to use the context to select the user id. 2025-12-30
    JourneyCreate journeyCreate = toJourneyCreate(0, journeyCreateRequest);
    Journey journey;
    try {
      journey = this->repository_->create(journeyCreate);
    } catch (std::exception &error) {
      return crow::response(500);
    }

    return sendJourneyResponse(crow::CREATED, journey);
  }

  crow::json::rvalue parseJson(const crow::request &request) {
    crow::json::rvalue body = crow::json::load(request.body);
    if (!body)
      throw std::invalid_argument("Body not JSON");
    return body;
  }

  response listJourneys(const context &context, const request &request) {
    std::vector<Journey> journeys = repository_->list();

    crow::json::wvalue::list jsonArray;
    jsonArray.reserve(journeys.size());
    for (const Journey &journey : journeys) {
      JourneyResponse journeyResponse = toJourneyResponse(journey);
      std::string journeyJson = api::json::toJson(journeyResponse).dump();
      jsonArray.push_back(journeyJson);
    }

    crow::json::wvalue outputJSON(jsonArray);
    crow::response response{outputJSON};
    response.code = crow::OK;
    response.set_header("Content-Type", "application/json");
    return response;
  }

  response getJourney(const context &context, const request &request, const int64_t id) {
    Journey journey;

    try {
      journey = repository_->get(id);
    } catch (std::invalid_argument &error) {
      return crow::response{crow::NOT_FOUND, "Not Found"};
    }

    return sendJourneyResponse(crow::OK, journey);
  }

  response sendJourneyResponse(const crow::status code, const Journey &journey) {
    JourneyResponse journeyResponse = toJourneyResponse(journey);
    response response;
    response.body = api::json::toJson(journeyResponse).dump();
    response.code = code;
    response.set_header("Content-Type", "application/json");
    return response;
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
      updatedJourney = repository_->update(id, journeyCreate);
    } catch (std::invalid_argument &error) {
      return crow::response(crow::NOT_FOUND);
    }
    return crow::response(crow::NO_CONTENT);
  }
};
