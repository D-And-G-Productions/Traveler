#include "App.h"
#include "http/controllers/HealthController.hpp"
#include "http/controllers/JourneyController.h"
#include "repository/MockJourneyRepository.h"
#include <memory>

crow::SimpleApp buildApp() {
  crow::SimpleApp app;

  std::shared_ptr<JourneyRepository> repository = std::make_shared<MockJourneyRepository>();
  std::shared_ptr<JourneyController> journeyController = std::make_shared<JourneyController>(repository);
  std::shared_ptr<HealthController> healthController = std::make_shared<HealthController>();
  journeyController->registerRoutes(app);
  healthController->registerRoutes(app);

  return app;
}
