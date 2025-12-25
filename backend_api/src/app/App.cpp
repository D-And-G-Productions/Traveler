#include "App.h"
#include "http/JourneyRoutes.h"
#include "repository/MockJourneyRepository.h"
#include <memory>

crow::SimpleApp buildApp() {
  crow::SimpleApp app;

  auto repo = std::make_shared<MockJourneyRepository>();
  registerJourneyRoutes(app, repo);

  return app;
}
