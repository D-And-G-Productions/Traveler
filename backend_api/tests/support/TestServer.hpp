#pragma once

#include "app/Server.hpp"
#include "authentication/TokenVerifier.hpp"
#include "http/controllers/HealthController.hpp"
#include "http/controllers/JourneyController.hpp"
#include "http/controllers/MeController.hpp"
#include "http/middleware/AuthMiddleware.hpp"
#include "repository/JourneyRepository.hpp"
#include "support/TestConstants.hpp"
#include "support/mocks/FakeUnitOfWork.hpp"
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
  shared_ptr<JourneyRepository> journeyRepo;
  shared_ptr<UserRepository> userRepo;
  shared_ptr<MeController> meController;
  shared_ptr<HealthController> healthController;
  shared_ptr<MockTokenVerifier> mockTokenVerifier;

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

protected:
  void stopServerThread() {
    if (serverThread.joinable()) {
      serverThread.join();
    }
  }

  void setupRepositories() override {
    journeyRepo = make_shared<MockJourneyRepository>();
    userRepo = make_shared<MockUserRepository>();
  }

  void setupMiddleware() override {
    AuthMiddleware &authMiddleware = application.get_middleware<AuthMiddleware>();
    mockTokenVerifier = make_shared<MockTokenVerifier>();
    authMiddleware.setVerifier(mockTokenVerifier);
    authMiddleware.setUserRepository(userRepo);
  }

  void setupControllers() override {
    shared_ptr<UnitOfWork> uow = make_shared<FakeUnitOfWork>();
    shared_ptr<JourneyService> journeyService = make_shared<JourneyService>(uow, journeyRepo);
    journeyController = make_shared<JourneyController>(journeyRepo, userRepo, journeyService);
    healthController = make_shared<HealthController>();
    meController = make_shared<MeController>(userRepo);
  }

  void registerRoutes() override {
    journeyController->registerRoutes(application);
    healthController->registerRoutes(application);
    meController->registerRoutes(application);
  }

  void acceptAllTokens() { mockTokenVerifier->acceptAll(); }

  void rejectAllTokens() { mockTokenVerifier->rejectAll(); }
};
