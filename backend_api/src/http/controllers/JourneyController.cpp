#include "JourneyController.h"
#include "json/JourneyJson.h"
#include <crow/app.h>
#include <crow/common.h>
#include <crow/http_request.h>
#include <crow/http_response.h>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <stdexcept>

using crow::HTTPMethod::POST, crow::HTTPMethod::PUT, crow::HTTPMethod::GET;

JourneyController::JourneyController(std::shared_ptr<JourneyRepository> repository) : repository_(repository) {}

void JourneyController::registerRoutes(crow::SimpleApp &app) {
  CROW_ROUTE(app, "/journey").methods(POST)([this](const crow::request &request) {
    return this->createJourney(request);
  });

  CROW_ROUTE(app, "/journey").methods(GET)([this]() { return listJourneys(); });

  CROW_ROUTE(app, "/journey/<int>").methods(GET)([this](const long id) {
    return getJourney(static_cast<int64_t>(id));
  });

  CROW_ROUTE(app, "/journey/<int>").methods(PUT)([this](const crow::request &request, long id) {
    return updateJourney(request, static_cast<int64_t>(id));
  });
};

crow::json::rvalue parseJSON(const crow::request &request) {
  crow::json::rvalue body = crow::json::load(request.body);
  if (!body) throw std::invalid_argument("Body not JSON");
  return body;
}

crow::response JourneyController::createJourney(const crow::request &request) {
  crow::json::rvalue body;
  try {
    body = parseJSON(request);
  } catch (std::invalid_argument &error) {
    return crow::response(400, "Invalid JSON");
  }

  Journey journey;
  try {
    journey = JourneyJSON::toJourney(body);
  } catch (std::exception &error) {
    return crow::response(400, "Missing/invalid fields");
  }

  JourneyRecord journeyRecord;
  try {
    journeyRecord = this->repository_->create(journey);
  } catch (std::exception &error) {
    return crow::response(500, "Missing/invalid fields");
  }

  return sendJourneyRecord(journeyRecord);
}

crow::response JourneyController::listJourneys() {
  std::vector<JourneyRecord> journeysRecords = repository_->list();

  crow::json::wvalue::list jsonArray;
  jsonArray.reserve(journeysRecords.size());
  for (const JourneyRecord &journeyRecord : journeysRecords) {
    jsonArray.push_back(JourneyJSON::toJson(journeyRecord));
  }

  crow::json::wvalue outputJSON(jsonArray);
  crow::response response{outputJSON};
  response.code = crow::OK;
  response.set_header("Content-Type", "application/json");
  return response;
};

crow::response JourneyController::getJourney(const int64_t id) {
  JourneyRecord journeyRecord;

  try {
    journeyRecord = repository_->get(id);
  } catch (std::invalid_argument &error) {
    return crow::response{crow::NOT_FOUND, "Not Found"};
  }
  return sendJourneyRecord(journeyRecord);
};

crow::response JourneyController::sendJourneyRecord(JourneyRecord &journeyRecord) {
  crow::json::wvalue json = JourneyJSON::toJson(journeyRecord);
  crow::response response;
  response.body = json.dump();
  std::cout << response.body << std::endl;
  response.code = crow::OK;
  response.set_header("Content-Type", "application/json");
  return response;
}

crow::response JourneyController::updateJourney(const crow::request &request, const int64_t id) {
  crow::json::rvalue body = crow::json::load(request.body);
  Journey journey = JourneyJSON::toJourney(body);
  JourneyRecord journeyRecord{id, journey};

  try {
    journeyRecord = repository_->update(journeyRecord);
  } catch (std::invalid_argument &error) {
    return crow::response(crow::NOT_FOUND);
  }
  return crow::response(crow::NO_CONTENT);
};
