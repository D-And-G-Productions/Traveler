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
        .methods(PUT)([this, &app](const crow::request &request, long id) {
          context &context = app.get_context<AuthMiddleware>(request);
          return updateJourney(context, request, static_cast<int64_t>(id));
        });
  }

private:
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
      journey = this->journeyRepo->insert(journeyCreate);
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
      updatedJourney = journeyRepo->update(id, journeyCreate);
    } catch (std::invalid_argument &error) {
      return crow::response(crow::NOT_FOUND);
    }
    return crow::response(crow::NO_CONTENT);
  }
};
