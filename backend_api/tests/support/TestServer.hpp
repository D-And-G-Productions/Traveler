#pragma once

#include "app/Server.hpp"
#include "authentication/TokenVerifier.hpp"
#include "http/controllers/HealthController.hpp"
#include "http/controllers/JourneyController.hpp"
#include "http/controllers/MeController.hpp"
#include "http/middleware/AuthMiddleware.hpp"
#include "repository/JourneyRepository.hpp"
#include "support/TestConstants.hpp"
#include "support/mocks/MockJourneyRepository.hpp"
#include "support/mocks/MockTokenVerifier.hpp"
#include "support/mocks/MockUserRepository.hpp"
#include <crow.h>
#include <crow/app.h>
#include <crow/http_request.h>
#include <crow/http_response.h>
#include <memory>

using std::make_shared;
using std::shared_ptr;

class TestServer : public Server<AuthMiddleware> {
public:
  shared_ptr<JourneyController> journeyController;
  shared_ptr<JourneyRepository> journeyRepository;
  shared_ptr<UserRepository> userRepository;
  shared_ptr<MeController> meController;
  shared_ptr<HealthController> healthController;

private:
  std::thread serverThread;

public:
  TestServer() : Server(string(TestConstants::LOCAL_ADDRESS), TestConstants::TEST_PORT) {}
  ~TestServer() override { stop(); }

  void stop() override {
    if (running.exchange(false)) {
      application.stop();
      stopServerThread();
    }
  }

  void start() override {
    auto runApplication = [this] {
      application.bindaddr(ADDRESS);
      application.port(PORT);
      application.run();
    };
    serverThread = std::thread(runApplication);
    application.wait_for_server_start();
  }

  crow::response handleRequest(crow::request &request) {
    crow::response response;
    application.handle_full(request, response);
    return response;
  }

protected:
  void stopServerThread() {
    if (serverThread.joinable()) {
      serverThread.join();
    }
  }

  void setupMiddleware() override {
    shared_ptr<TokenVerifier> tv = make_shared<MockTokenVerifier>();
    AuthMiddleware &am = application.get_middleware<AuthMiddleware>();
    am.setVerifier(tv);
  }

  void setupRepositories() override {
    journeyRepository = make_shared<MockJourneyRepository>();
    userRepository = make_shared<MockUserRepository>();
  }

  void setupControllers() override {
    journeyController = make_shared<JourneyController>(journeyRepository);
    healthController = make_shared<HealthController>();
    meController = make_shared<MeController>(userRepository);
  }

  void registerRoutes() override {
    journeyController->registerRoutes(application);
    healthController->registerRoutes(application);
    meController->registerRoutes(application);
  }
};
