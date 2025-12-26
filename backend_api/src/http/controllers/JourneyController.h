#include "repository/JourneyRepository.h"
#include <crow/app.h>
#include <crow/common.h>
#include <crow/http_request.h>
#include <crow/http_response.h>
#include <cstdint>
#include <memory>

class JourneyController {
public:
  JourneyController(std::shared_ptr<JourneyRepository> repository);

  void registerRoutes(crow::SimpleApp &app);

private:
  std::shared_ptr<JourneyRepository> repository_;

  crow::response createJourney(const crow::request &request);

  crow::response listJourneys();

  crow::response getJourney(const int64_t id);

  crow::response updateJourney(const crow::request &request, const int64_t id);

  crow::response sendJourneyRecord(JourneyRecord &journeyRecord);
};
