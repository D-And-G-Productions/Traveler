#pragma once

#include "app/App.hpp"

class Controller
{
public:
  virtual void registerRoutes(TravelerApp &app) = 0;
};
