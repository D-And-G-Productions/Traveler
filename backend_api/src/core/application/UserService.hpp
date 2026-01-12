#pragma once

#include "domain/User.hpp"
#include "persistence/DBPool.hpp"
#include <cstdint>

class UserService
{
public:
  UserService(std::shared_ptr<DBPool> p) : pool(std::move(p)) {}

  User getUser(const std::string subject) const;
  User getUser(const int64_t userId) const;

private:
  std::shared_ptr<DBPool> pool;
};
