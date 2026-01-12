#pragma once

#include "Store.hpp"
#include "domain/User.hpp"
#include <string_view>

class UserStore : public Store
{
public:
  using Store::Store;
  User selectUser(const std::string_view subject) const;
  User selectUser(const std::int64_t id) const;
  User insertUserBySubject(const std::string_view subject);
};
