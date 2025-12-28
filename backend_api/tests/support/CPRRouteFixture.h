#pragma once

#include "support/CrowTestServer.h"
#include <cpr/cpr.h>
#include <gtest/gtest.h>
#include <string>

class CPRRouteFixture : public ::testing::Test {
protected:
  std::unique_ptr<CrowTestServer> server;

  void SetUp() override;

  void TearDown() override;

  cpr::Response post(const std::string &url, const std::string &body) const;

  cpr::Response get(const std::string &url) const;
};
