#include "CrowTestServer.h"
#include "http/JourneyRoutes.h"
#include "repository/JourneyRepository.h"
#include "repository/MockJourneyRepository.h"
#include <memory>

CrowTestServer::CrowTestServer(int port) : port_(port), repository_(std::make_shared<MockJourneyRepository>()) {
  registerRoutes();
}

void CrowTestServer::registerRoutes() {
  registerJourneyRoutes(app_, repository_);
  app_.validate();
}

CrowTestServer::~CrowTestServer() { stop(); }

void CrowTestServer::stop() {
  if (!stopped_.exchange(true)) {
    app_.stop();
    if (server_thread_.joinable()) {
      server_thread_.join();
    }
  }
}

crow::SimpleApp &CrowTestServer::app() { return app_; }

void CrowTestServer::start() {
  server_thread_ = std::thread([this] { app_.bindaddr("127.0.0.1").port(port_).run(); });
  app_.wait_for_server_start();
}

std::string CrowTestServer::baseUrl() const { return "http://127.0.0.1:" + std::to_string(port_); }

std::shared_ptr<JourneyRepository> CrowTestServer::repository() { return repository_; }
