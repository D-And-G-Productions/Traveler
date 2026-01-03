#pragma once

#include "api/dto/JourneyCreateRequest.hpp"
#include "application/JourneyService.hpp"
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
using crow::HTTPMethod::GET;
using crow::HTTPMethod::POST;
using crow::HTTPMethod::PUT;
using std::shared_ptr;

class JourneyController {
private:
  shared_ptr<JourneyRepository> journeyRepo;
  shared_ptr<UserRepository> userRepo;
  shared_ptr<JourneyService> journeyService;

public:
  JourneyController(
      shared_ptr<JourneyRepository> journeyRepo_,
      shared_ptr<UserRepository> userRepo_,
      shared_ptr<JourneyService> journeyService_
  ) : journeyRepo(journeyRepo_), userRepo(userRepo_), journeyService(journeyService_) {}

  void registerRoutes(crow::App<AuthMiddleware> &app);

private:
  struct FailedToReadJson : public std::runtime_error {
    using std::runtime_error::runtime_error;
  };

  struct ConversionFromJsonFailed : public std::runtime_error {
    using std::runtime_error::runtime_error;
  };

  crow::response createJourney(const context &context, const crow::request &request);
  crow::response listJourneys(const context &context, const crow::request &request);
  crow::response getJourney(const context &context, const crow::request &request, const int64_t id);
  crow::response
  updateJourney(const context &context, const crow::request &request, const int64_t id);
  crow::json::rvalue parseJson(const std::string body);
  JourneyCreateRequest jsonToJourneyCreateRequest(const crow::json::rvalue json);
  crow::response buildResponseWithJourneyBody(const crow::status code, const Journey &journey);
  crow::json::wvalue toJsonArray(const std::vector<Journey> &journeys);
};
