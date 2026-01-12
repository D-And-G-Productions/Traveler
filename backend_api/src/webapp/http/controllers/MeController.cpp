#include "MeController.hpp"
#include "api/Mappings.hpp"
#include "api/dto/UserReponseContract.hpp"
#include "api/json/JsonConversion.hpp"

using crow::HTTPMethod::GET;

void MeController::registerRoutes(TravelerApp &app)
{
  CROW_ROUTE(app, "/me")
      .CROW_MIDDLEWARES(app, AuthMiddleware)
      .methods(GET)(
          [this, &app](const crow::request &request)
          {
            AuthMiddleware::context &authContext = app.get_context<AuthMiddleware>(request);
            return queryMe(authContext, request);
          }
      );
}

crow::response
MeController::queryMe(AuthMiddleware::context &authContext, const crow::request &request)
{
  UserResponseContract contract = ContractMappings::toUserResponseContract(authContext.user);
  crow::json::wvalue json = JsonConversion::toJson<UserResponseContract>(contract);
  return crow::response{crow::OK, "application/json", json.dump()};
};
