#pragma once
#include "domain/User.hpp"
#include "domain/UserUpdate.hpp"
#include <string>

using std::string;

// TODO: Reshape class to use primarily ID.
class UserRepository {
public:
  virtual ~UserRepository() = default;

  virtual User get(const std::string &subject) const = 0;

  virtual User update(const int64_t id, const UserUpdate &userUpdate) = 0;

  virtual User update(const string &subject, const UserUpdate &userUpdate) = 0;

  virtual User insert(const std::string &subject) = 0;
};
