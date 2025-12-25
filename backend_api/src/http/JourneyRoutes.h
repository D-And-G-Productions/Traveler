#pragma once
#include "../repository/JourneyRepository.h"
#include <crow.h>

void registerJourneyRoutes(crow::SimpleApp &app, std::shared_ptr<JourneyRepository> repo);
