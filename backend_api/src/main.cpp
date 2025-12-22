#include <crow.h>
#include <crow/common.h>
#include <crow/http_response.h>
#include <crow/json.h>
#include <crow/returnable.h>
#include <memory>
#include <vector>

struct Journey {
  std::string source;
  std::string destination;
  std::string datetime;
};

class DatabaseController {
protected:
  std::vector<Journey> journeys;

public:
  virtual ~DatabaseController() = default;
  virtual void addJourney(Journey journey) = 0;
  virtual const std::vector<Journey> &getJourneys() const = 0;
};

class MockDatabase : public DatabaseController {
public:
  void addJourney(Journey journey) override { journeys.push_back(journey); }
  const std::vector<Journey> &getJourneys() const override { return journeys; }
};

crow::json::wvalue::list getArrayOfJourneys(std::vector<Journey> journeys) {
  crow::json::wvalue::list arr;
  arr.reserve(journeys.size());

  for (const Journey &journey : journeys) {
    crow::json::wvalue item;
    item["source"] = journey.source;
    item["destination"] = journey.destination;
    item["datetime"] = journey.datetime;
    arr.push_back(std::move(item));
  }

  return arr;
}

auto createGETJourneyResponse(crow::json::wvalue::list arrayOfJourneys) {
  crow::json::wvalue out(arrayOfJourneys);
  crow::response res{out};
  res.code = crow::OK;
  res.set_header("Content-Type", "application/json");
  return res;
}

int main() {
  crow::SimpleApp app;

  const std::unique_ptr<DatabaseController> &databaseController = std::make_unique<MockDatabase>();

  CROW_ROUTE(app, "/journey").methods(crow::HTTPMethod::Post)([&databaseController](const crow::request &req) {
    crow::json::rvalue body = crow::json::load(req.body);
    if (!body) {
      return crow::response(400, "Invalid JSON");
    }

    Journey journey{body["source"].s(), body["destination"].s(), body["datetime"].s()};
    databaseController->addJourney(journey);
    return crow::response(crow::OK);
  });

  CROW_ROUTE(app, "/journey").methods(crow::HTTPMethod::Get)([&databaseController]() {
    const std::vector<Journey> &journeys = databaseController->getJourneys();
    crow::json::wvalue::list arrayOfJourneys = getArrayOfJourneys(journeys);
    crow::response response = createGETJourneyResponse(arrayOfJourneys);
    return response;
  });

  app.port(18080).multithreaded().run();
}
