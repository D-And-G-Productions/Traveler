#pragma once

#include "application/UnitOfWork.hpp"

class FakeUnitOfWork : public UnitOfWork {
public:
  void run(const std::function<void()> &fn) override { fn(); }
};
