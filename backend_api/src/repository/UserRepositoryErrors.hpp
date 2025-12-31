#include <stdexcept>

class UserDoesNotExist : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

class UserAlreadyExists : public std::runtime_error {
  using std::runtime_error::runtime_error;
};
