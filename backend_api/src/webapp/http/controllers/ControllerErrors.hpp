#pragma once

#include <stdexcept>

struct NotVerfiedError : public std::runtime_error
{
  using std::runtime_error::runtime_error;
};

struct FailedToLoadJson : public std::runtime_error
{
  using std::runtime_error::runtime_error;
};
