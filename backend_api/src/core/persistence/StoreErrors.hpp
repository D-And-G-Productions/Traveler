#pragma once

#include <stdexcept>

struct JourneyNotFoundError : public std::runtime_error
{
  using std::runtime_error::runtime_error;
};

struct UserDoesNotExist : public std::runtime_error
{
  using std::runtime_error::runtime_error;
};

struct UserAlreadyExists : public std::runtime_error
{
  using std::runtime_error::runtime_error;
};
