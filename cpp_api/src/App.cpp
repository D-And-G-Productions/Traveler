#include <oatpp/Environment.hpp>
#include <oatpp/base/Log.hpp>

#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/web/server/HttpRequestHandler.hpp>
#include <oatpp/web/server/HttpRouter.hpp>

#include <oatpp/network/Server.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>

#include <oatpp/json/ObjectMapper.hpp>

#include "controller/TravelController.hpp"

class Handler : public oatpp::web::server::HttpRequestHandler {
public:
  std::shared_ptr<OutgoingResponse>
  handle(const std::shared_ptr<IncomingRequest> &) override {
    return ResponseFactory::createResponse(Status::CODE_200, "Twin Baddies");
  }
};

void run() {
  auto router = oatpp::web::server::HttpRouter::createShared();

  router->route("GET", "/hello", std::make_shared<Handler>());

  auto objectMapper = oatpp::json::ObjectMapper::createShared();
  router->addController(std::make_shared<TravelController>(objectMapper));

  auto connectionHandler =
      oatpp::web::server::HttpConnectionHandler::createShared(router);

  auto connectionProvider =
      oatpp::network::tcp::server::ConnectionProvider::createShared(
          {"0.0.0.0", 8000, oatpp::network::Address::IP_4});

  oatpp::network::Server server(connectionProvider, connectionHandler);

  OATPP_LOGi("MyApp", "Server running on port {}",
             connectionProvider->getProperty("port").toString());

  server.run();
}

int main() {
  oatpp::Environment::init();
  run();
  oatpp::Environment::destroy();
  return 0;
}
