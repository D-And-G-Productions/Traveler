#pragma once

#include "http/controllers/Controller.hpp"

class HealthController : public Controller
{
public:
  void registerRoutes(TravelerApp &app) override;
};
