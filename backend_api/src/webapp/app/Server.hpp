#pragma once

#include "app/App.hpp"
#include "http/controllers/HealthController.hpp"
#include "http/controllers/JourneyController.hpp"
#include "http/controllers/MeController.hpp"
#include "persistence/DBPool.hpp"

#include <atomic>
#include <memory>
#include <optional>
#include <string>
#include <utility>

class Server
{
public:
  explicit Server(std::string dbUrl) : databaseUrl(std::move(dbUrl)) {}

  Server(const Server &) = delete;

  Server &operator=(const Server &) = delete;

  virtual ~Server() = default;

  bool isRunning() const noexcept;

  void initialise();

  void start();

  void stop() noexcept;

protected:
  std::shared_ptr<DBPool> pool;
  std::shared_ptr<TokenVerifier> tokenVerifier;

  TravelerApp &app();

  virtual std::shared_ptr<TokenVerifier> produceTokenVerifier() = 0;

  virtual void configureApp(TravelerApp & /*app*/);

  virtual void runApp(TravelerApp &app);

  virtual void afterStop() noexcept;

private:
  static constexpr int DEFAULT_PORT = 18080;
  static constexpr int POOL_SIZE = 3;
  std::atomic_bool running{false};
  std::string databaseUrl;
  std::optional<TravelerApp> application;
  std::shared_ptr<HealthController> healthController;
  std::shared_ptr<MeController> meController;
  std::shared_ptr<JourneyController> journeyController;

  void requireInitialised() const;

  void setupMiddleware();

  void setupControllers();

  void registerRoutes();
};
