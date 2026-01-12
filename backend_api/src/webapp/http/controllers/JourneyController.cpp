#include "JourneyController.hpp"
#include "api/Mappings.hpp"
#include "api/json/JsonConversion.hpp"
#include "http/controllers/ControllerErrors.hpp"
#include "persistence/StoreErrors.hpp"
#include <crow/common.h>

using crow::HTTPMethod::DELETE;
using crow::HTTPMethod::GET;
using crow::HTTPMethod::POST;
using crow::HTTPMethod::PUT;

namespace
{
crow::json::wvalue toJsonArray(const std::vector<Journey> &journeys)
{
  crow::json::wvalue::list journeysJson(journeys.size());

  auto fn = [](const Journey &j) -> crow::json::wvalue
  {
    JourneyResponseContract contract = ContractMappings::toJourneyResponseContract(j);
    return JsonConversion::toJson<JourneyResponseContract>(contract);
  };
  std::transform(journeys.begin(), journeys.end(), journeysJson.begin(), fn);
  return journeysJson;
}

crow::json::rvalue loadJson(const std::string body)
{
  crow::json::rvalue json = crow::json::load(body);
  if (!json)
    throw FailedToLoadJson("Invalid Json");
  return json;
}

std::string writeJourneyToBody(Journey &j)
{
  JourneyResponseContract responseContract = ContractMappings::toJourneyResponseContract(j);
  return JsonConversion::toJson(responseContract).dump();
}
} // namespace

void JourneyController::registerRoutes(TravelerApp &app)
{
  CROW_ROUTE(app, "/journey/<int>")
      .CROW_MIDDLEWARES(app, AuthMiddleware)
      .methods(GET)(
          [this, &app](const crow::request &request, const long id)
          {
            AuthMiddleware::context &authContext = app.get_context<AuthMiddleware>(request);
            return getJourney(authContext, request, static_cast<int64_t>(id));
          }
      );

  CROW_ROUTE(app, "/journey")
      .CROW_MIDDLEWARES(app, AuthMiddleware)
      .methods(GET)(
          [this, &app](const crow::request &request)
          {
            AuthMiddleware::context &authContext = app.get_context<AuthMiddleware>(request);
            return getJourneys(authContext, request);
          }
      );

  CROW_ROUTE(app, "/journey")
      .CROW_MIDDLEWARES(app, AuthMiddleware)
      .methods(POST)(
          [this, &app](const crow::request &request)
          {
            AuthMiddleware::context &authContext = app.get_context<AuthMiddleware>(request);
            return postJourney(authContext, request);
          }
      );

  CROW_ROUTE(app, "/journey/<int>")
      .CROW_MIDDLEWARES(app, AuthMiddleware)
      .methods(PUT)(
          [this, &app](const crow::request &request, long id)
          {
            AuthMiddleware::context &authContext = app.get_context<AuthMiddleware>(request);
            return updateJourney(authContext, request, static_cast<int64_t>(id));
          }
      );

  CROW_ROUTE(app, "/journey/<int>")
      .CROW_MIDDLEWARES(app, AuthMiddleware)
      .methods(DELETE)(
          [this, &app](const crow::request &request, const long id)
          {
            AuthMiddleware::context &authContext = app.get_context<AuthMiddleware>(request);
            return deleteJourney(authContext, request, static_cast<int64_t>(id));
          }
      );
}

crow::response JourneyController::getJourney(
    AuthMiddleware::context &authContext,
    const crow::request &request,
    const int64_t id
)
{
  try
  {
    Journey journey = journeyService->getJourney(id, authContext.user.id);
    JourneyResponseContract responseContract = ContractMappings::toJourneyResponseContract(journey);
    std::string body = writeJourneyToBody(journey);
    return crow::response{crow::OK, "application/json", body};
  }
  catch (JourneyNotFoundError &)
  {
    return crow::response{crow::NOT_FOUND};
  }
}

crow::response
JourneyController::getJourneys(AuthMiddleware::context &authContext, const crow::request &request)
{
  std::vector<Journey> journeys = journeyService->getJourneys(authContext.user.id);
  crow::json::wvalue json = toJsonArray(journeys);
  return crow::response{crow::OK, "application/json", json.dump()};
};

namespace
{
JourneyCreate readCreateFromBody(const int64_t userId, const std::string &body)
{
  crow::json::rvalue json = loadJson(body);
  JourneyCreateRequestContract contract =
      JsonConversion::fromJson<JourneyCreateRequestContract>(json);
  return ContractMappings::toJourneyCreate(userId, contract);
}
} // namespace

crow::response
JourneyController::postJourney(AuthMiddleware::context &authContext, const crow::request &request)
{
  try
  {
    JourneyCreate journeyCreate = readCreateFromBody(authContext.user.id, request.body);
    Journey journey = journeyService->addJourney(journeyCreate);
    std::string body = writeJourneyToBody(journey);
    return crow::response{crow::CREATED, "application/json", body};
  }
  catch (FailedToLoadJson &e)
  {
    return crow::response(crow::status::BAD_REQUEST, e.what());
  }
  catch (JsonParseError &e)
  {
    return crow::response(crow::BAD_REQUEST, e.what());
  }
}

namespace
{
JourneyUpdate readUpdateFromBody(const std::string &body)
{
  crow::json::rvalue json = loadJson(body);
  JourneyUpdateRequestContract contract =
      JsonConversion::fromJson<JourneyUpdateRequestContract>(json);
  return ContractMappings::toJourneyUpdate(contract);
}
} // namespace

crow::response JourneyController::updateJourney(
    AuthMiddleware::context &authContext,
    const crow::request &request,
    const int64_t id
)
{
  try
  {
    JourneyUpdate update = readUpdateFromBody(request.body);
    Journey journey = journeyService->updateJourney(id, authContext.user.id, update);
    std::string body = writeJourneyToBody(journey);
    return crow::response{crow::NO_CONTENT, "application/json", body};
  }
  catch (FailedToLoadJson &e)
  {
    return crow::response(crow::status::BAD_REQUEST, e.what());
  }
  catch (JsonParseError &e)
  {
    return crow::response(crow::BAD_REQUEST, e.what());
  }
  catch (JourneyNotFoundError &)
  {
    return crow::response(crow::NOT_FOUND);
  }
}

crow::response JourneyController::deleteJourney(
    AuthMiddleware::context &authContext,
    const crow::request &,
    const int64_t id
)
{
  try
  {
    journeyService->deleteJourney(id, authContext.user.id);
    return crow::response(crow::NO_CONTENT);
  }
  catch (JourneyNotFoundError &)
  {
    return crow::response(crow::NOT_FOUND);
  }
}
