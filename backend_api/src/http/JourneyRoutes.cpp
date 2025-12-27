#include "JourneyRoutes.h"
#include "domain/Journey.h"
#include "repository/JourneyRepository.h"
#include "json/JourneyJson.h"
#include <crow/common.h>
#include <crow/http_response.h>
#include <crow/json.h>
#include <cstdint>
#include <exception>
#include <stdexcept>

crow::response sendJourneyRecord(JourneyRecord &journeyRecord) {
  crow::json::wvalue json = JourneyJSON::toJson(journeyRecord);
  crow::response response;
  response.body = json.dump();
  response.code = crow::OK;
  response.set_header("Content-Type", "application/json");
  return response;
}

void registerJourneyRoutes(crow::SimpleApp &app, std::shared_ptr<JourneyRepository> repository) {
  CROW_ROUTE(app, "/journey").methods(crow::HTTPMethod::POST)([repository](const crow::request &request) {
    crow::json::rvalue body = crow::json::load(request.body);
    if (!body) return crow::response(400, "Invalid JSON");

    Journey journey;
    try {
      journey = JourneyJSON::toJourney(body);
    } catch (std::exception error) {
      return crow::response(400, "Missing/invalid fields");
    }
    repository->create(std::move(journey));
    return crow::response(crow::OK);
  });

  CROW_ROUTE(app, "/journey").methods(crow::HTTPMethod::GET)([repository]() {
    std::vector<JourneyRecord> journeysRecords = repository->list();

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
  });

  CROW_ROUTE(app, "/journey/<int>").methods(crow::HTTPMethod::GET)([repository](int id) {
    JourneyRecord journeyRecord;

    try {
      journeyRecord = repository->get(id);
    } catch (std::invalid_argument error) {
      return crow::response{crow::NOT_FOUND, "Not Found"};
    }
    return sendJourneyRecord(journeyRecord);
  });

  CROW_ROUTE(app, "/journey/<int>")
      .methods(crow::HTTPMethod::PUT)([repository](const crow::request &request, int64_t id) {
        crow::json::rvalue body = crow::json::load(request.body);
        Journey journey = JourneyJSON::toJourney(body);
        JourneyRecord journeyRecord{id, journey};

        try {
          journeyRecord = repository->update(journeyRecord);
        } catch (std::invalid_argument error) {
          return crow::response(crow::NOT_FOUND);
        }
        return crow::response(crow::NO_CONTENT);
      });

  CROW_ROUTE(app, "/health").methods(crow::HTTPMethod::Get)([repository]() { return crow::response(crow::OK); });
}
