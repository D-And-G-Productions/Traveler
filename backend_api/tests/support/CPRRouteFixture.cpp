#include "CPRRouteFixture.h"
#include "TestConstants.h"

void CPRRouteFixture::SetUp() {
  server = std::make_unique<CrowTestServer>(TestConstants::TEST_PORT);
  server->start();
}

void CPRRouteFixture::TearDown() {
  server->stop();
  server.reset();
}

cpr::Response CPRRouteFixture::postJson(const std::string &url, const std::string &body) const {
  cpr::Url requestUrl = {url};
  cpr::Body requestBody = {body};
  cpr::Header requestHeader = {{"Content-Type", "application/json"}};
  cpr::Timeout requestTimeout = {TestConstants::TIMEOUT_MS};
  return cpr::Post(requestUrl, requestBody, requestHeader, requestTimeout);
}

cpr::Response CPRRouteFixture::get(const std::string &url) const {
  cpr::Url requestUrl = {url};
  cpr::Timeout requestTimeout = {TestConstants::TIMEOUT_MS};
  return cpr::Get(requestUrl, requestTimeout);
}
