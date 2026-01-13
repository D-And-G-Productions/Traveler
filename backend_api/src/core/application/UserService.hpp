#pragma once

#include "domain/User.hpp"
#include "persistence/DBPool.hpp"
#include <cstdint>
#include <string_view>

class UserService
{
public:
  UserService(std::shared_ptr<DBPool> p) : pool(std::move(p)) {}

  User getUser(const std::string_view subject) const;
  User getUser(const int64_t userId) const;
  User insertUser(const std::string_view subject);

private:
  std::shared_ptr<DBPool> pool;
};
