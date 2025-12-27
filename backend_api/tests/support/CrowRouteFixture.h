#pragma once

#include "TestConstants.h"
#include "support/CrowTestServer.h"
#include <gtest/gtest.h>

class CrowRouteFixture : public ::testing::Test {
protected:
  std::unique_ptr<CrowTestServer> server;

  void SetUp() override { server = std::make_unique<CrowTestServer>(TestConstants::TEST_PORT); }

  void TearDown() override { server.reset(); }
};
