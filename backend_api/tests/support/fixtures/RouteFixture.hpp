#pragma once

#include "support/TestServer.hpp"
#include <gtest/gtest.h>
#include <string>

class RouteFixture : public ::testing::Test {
public:
  std::string url;

protected:
  std::unique_ptr<TestServer> server;

  void SetUp() override {
    server = std::make_unique<TestServer>();
    url = std::format("http://{}:{}", server->ADDRESS, server->PORT);
    server->initialise();
    server->start();
  }

  void TearDown() override {
    server->stop();
    server.reset();
  }
};
