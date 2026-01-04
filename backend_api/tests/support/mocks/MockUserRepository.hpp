#pragma once

#include "domain/User.hpp"
#include "domain/UserUpdate.hpp"
#include "repository/UserRepository.hpp"
#include "repository/UserRepositoryErrors.hpp"
#include <cstdint>
#include <format>
#include <mutex>
#include <string>
#include <unordered_map>

using std::string;
using Iterator = std::unordered_map<string, User>::iterator;
using ConstIterator = std::unordered_map<string, User>::const_iterator;

class MockUserRepository final : public UserRepository {
public:
  User get(const string &subject) const override {
    std::lock_guard<std::mutex> lock(mutex);
    ConstIterator iterator = users.find(subject);
    requireUserExists(iterator, subject);
    return iterator->second;
  }

  User update(const int64_t id, const UserUpdate &userUpdate) override {
    std::lock_guard<std::mutex> lock(mutex);
    auto iterator = idToSubject.find(id);
    requireUserExists(iterator, id);
    std::string subject = iterator->second;
    return updateBySubject(subject, userUpdate);
  }

  User update(const string &subject, const UserUpdate &userUpdate) override {
    std::lock_guard<std::mutex> lock(mutex);
    return updateBySubject(subject, userUpdate);
  }

  User insert(const std::string &subject) override {
    std::lock_guard<std::mutex> lock(mutex);
    auto [iterator, success] = users.emplace(subject, User{.id = id, .sub = subject});

    if (!success) {
      string errorMessage = std::format("User with subject '{}' already exists", subject);
      throw UserAlreadyExists(errorMessage);
    }
    idToSubject.emplace(id, subject);
    id++;
    return iterator->second;
  }

private:
  std::unordered_map<string, User> users;
  std::unordered_map<int64_t, string> idToSubject;
  mutable std::mutex mutex;
  int64_t id{0};

  void requireUserExists(const ConstIterator &iterator, const std::string &subject) const {
    if (iterator == users.end()) {
      string errorMessage = std::format("User with subject '{}' does not exist.", subject);
      throw UserDoesNotExist(errorMessage);
    }
  }

  void requireUserExists(const auto &iterator, const int64_t id) const {
    if (iterator == idToSubject.end()) {
      string errorMessage = std::format("User with id '{}' does not exist.", id);
      throw UserDoesNotExist(errorMessage);
    }
  }

  Iterator findUserBySubject(const std::string &subject) {
    Iterator iterator = users.find(subject);
    requireUserExists(iterator, subject);
    return iterator;
  }

  User updateBySubject(const string &subject, const UserUpdate &userUpdate) {
    Iterator iterator = findUserBySubject(subject);
    User &user = iterator->second;
    updateUser(user, userUpdate);
    return user;
  }

  void updateUser(User &user, const UserUpdate &userUpdate) {
    user.name = userUpdate.name;
    user.telephone = userUpdate.telephone;
    // TODO: Dynamically set time.
    user.updatedAt = "NOW";
  }
};
