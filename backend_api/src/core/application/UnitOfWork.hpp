#pragma once

#include <functional>

class UnitOfWork {
public:
  virtual ~UnitOfWork() = default;
  virtual void run(const std::function<void()> &fn) = 0;
};
