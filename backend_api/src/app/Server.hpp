#pragma once

#include <atomic>
#include <crow.h>
#include <crow/app.h>
#include <crow/http_request.h>
#include <crow/http_response.h>
#include <memory>
#include <string>

using std::shared_ptr;

template <typename... Middlewares>
class Server {
public:
  const std::string ADDRESS;
  const int PORT;

protected:
  crow::App<Middlewares...> application;
  std::atomic_bool running{true};

public:
  Server(const std::string &address, const int &port) : ADDRESS(address), PORT(port) {}

  Server(const Server &) = delete;

  Server &operator=(const Server &) = delete;

  virtual ~Server() = default;

  virtual void stop() = 0;

  virtual void start() = 0;

  void initialise() {
    setupMiddleware();
    setupRepositories();
    setupControllers();
    registerRoutes();
    application.validate();
  }

protected:
  virtual void setupMiddleware() = 0;

  virtual void setupRepositories() = 0;

  virtual void setupControllers() = 0;

  virtual void registerRoutes() = 0;
};
