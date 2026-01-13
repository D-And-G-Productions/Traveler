#include "Server.hpp"
#include "persistence/EnvVariables.hpp"

TravelerApp &Server::app()
{
  requireInitialised();
  return *application;
}

void Server::requireInitialised() const
{
  if (!application)
  {
    throw std::logic_error("Server not initialised. Call initialise() before start().");
  }
}

bool Server::isRunning() const noexcept { return running.load(); }

void Server::initialise()
{
  pool = produceDBPool();
  application.emplace();

  setupMiddleware();
  setupControllers();
  registerRoutes();

  configureApp(*application);
  application->validate();
}

void Server::setupMiddleware()
{
  tokenVerifier = produceTokenVerifier();

  auto userService = std::make_shared<UserService>(pool);
  AuthMiddleware &authMiddleware = application->get_middleware<AuthMiddleware>();
  authMiddleware.setDeps(tokenVerifier, std::move(userService));
}

void Server::setupControllers()
{
  healthController = std::make_shared<HealthController>();

  auto userService = std::make_shared<UserService>(pool);
  meController = std::make_shared<MeController>(std::move(userService));

  auto journeyService = std::make_shared<JourneyService>(pool);
  journeyController = std::make_shared<JourneyController>(std::move(journeyService));
}

void Server::registerRoutes()
{
  healthController->registerRoutes(*application);
  meController->registerRoutes(*application);
  journeyController->registerRoutes(*application);
}

void Server::configureApp(TravelerApp &) {}

void Server::start()
{
  requireInitialised();
  running.store(true);
  runApp(*application);
}

void Server::runApp(TravelerApp &app) { app.port(DEFAULT_PORT).multithreaded().run(); }

void Server::stop() noexcept
{
  if (!running.exchange(false))
  {
    return;
  }

  if (application)
  {
    application->stop();
  }

  afterStop();
}

void Server::afterStop() noexcept {}

std::shared_ptr<DBPool> Server::produceDBPool()
{
  return std::make_shared<DBPool>(EnvVariables::getVar("DATABASE_URL"), POOL_SIZE);
};
