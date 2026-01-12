#pragma once

#include "app/Server.hpp"

#include <cstdint>
#include <thread>

class TestServer final : public Server
{
public:
  using Server::Server;

  ~TestServer() override { stop(); }

  User createUserFromToken(const std::string &token);
  Journey createJourney(const JourneyCreate &jc);
  Journey journeyFromDB(const int64_t journeyId);

protected:
  std::shared_ptr<TokenVerifier> produceTokenVerifier() override;
  void configureApp(TravelerApp &app) override;
  void runApp(TravelerApp &app) override;
  void afterStop() noexcept override;

private:
  std::thread serverThread;

  void joinThread() noexcept;
};
