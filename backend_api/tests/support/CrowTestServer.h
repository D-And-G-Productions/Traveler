#pragma once

#include "repository/JourneyRepository.h"
#include <atomic>
#include <crow.h>
#include <crow/app.h>
#include <memory>
#include <string>
#include <thread>

class CrowTestServer {
  int port_;
  crow::SimpleApp app_;
  std::thread server_thread_;
  std::atomic<bool> stopped_{false};
  std::shared_ptr<JourneyRepository> repository_;

public:
  CrowTestServer(int port);

  void registerRoutes();

  ~CrowTestServer();

  void stop();

  CrowTestServer(const CrowTestServer &) = delete;

  CrowTestServer &operator=(const CrowTestServer &) = delete;

  void start();

  crow::SimpleApp &app();

  std::shared_ptr<JourneyRepository> repository();

  std::string baseUrl() const;
};
