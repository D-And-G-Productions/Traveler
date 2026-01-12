#pragma once

#include "application/JourneyService.hpp"
#include "http/controllers/Controller.hpp"

class JourneyController : public Controller
{
public:
  JourneyController(std::shared_ptr<JourneyService> js) : journeyService(std::move(js)) {}

  void registerRoutes(TravelerApp &app) override;

private:
  std::shared_ptr<JourneyService> journeyService;

  crow::response
  getJourney(AuthMiddleware::context &authContext, const crow::request &request, const int64_t id);
  crow::response getJourneys(AuthMiddleware::context &authContext, const crow::request &request);
  crow::response postJourney(AuthMiddleware::context &authContext, const crow::request &request);
  crow::response updateJourney(
      AuthMiddleware::context &authContext,
      const crow::request &request,
      const int64_t id
  );
  crow::response deleteJourney(
      AuthMiddleware::context &authContext,
      const crow::request &request,
      const int64_t id
  );
};
