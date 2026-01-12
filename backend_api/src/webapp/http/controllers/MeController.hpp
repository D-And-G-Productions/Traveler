#pragma once

#include "http/controllers/Controller.hpp"

class MeController : public Controller
{
public:
  MeController(std::shared_ptr<UserService> s) : service(s) {}

  void registerRoutes(TravelerApp &app) override;

private:
  std::shared_ptr<UserService> service;

  crow::response queryMe(AuthMiddleware::context &authContext, const crow::request &request);
};
