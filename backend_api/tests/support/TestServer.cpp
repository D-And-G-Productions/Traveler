#include "TestServer.hpp"
#include "TestConstants.hpp"
#include "app/Server.hpp"
#include "mocks/MockTokenVerifier.hpp"
#include "persistence/JourneyStore.hpp"
#include "persistence/UserStore.hpp"

User TestServer::createUserFromToken(const std::string &token)
{
  Verification verification = tokenVerifier->verifyAccessToken(token);

  DBPool::Lease db = pool->acquire();
  pqxx::work tx{db.connection()};

  UserStore userStore{tx};
  User user = userStore.insertUserBySubject(verification.subject);

  tx.commit();
  return user;
}

Journey TestServer::createJourney(const JourneyCreate &jc)
{
  DBPool::Lease lease = pool->acquire();
  pqxx::work tx{lease.connection()};

  JourneyStore journeyStore{tx};
  Journey journey = journeyStore.insertJourney(jc);

  tx.commit();
  return journey;
}

Journey TestServer::journeyFromDB(const int64_t journeyId)
{
  DBPool::Lease lease = pool->acquire();
  pqxx::read_transaction tx{lease.connection()};

  JourneyStore journeyStore{tx};
  return journeyStore.selectJourney(journeyId);
}

std::shared_ptr<TokenVerifier> TestServer::produceTokenVerifier()
{
  return std::make_shared<MockTokenVerifier>(pool);
}

void TestServer::configureApp(TravelerApp &app)
{
  app.bindaddr(static_cast<std::string>(TestConstants::LOCAL_ADDRESS));
  app.port(TestConstants::TEST_PORT);
}

void TestServer::runApp(TravelerApp &app)
{
  serverThread = std::thread([&app] { app.run(); });
  app.wait_for_server_start();
}

void TestServer::afterStop() noexcept { joinThread(); }

void TestServer::joinThread() noexcept
{
  if (serverThread.joinable())
  {
    serverThread.join();
  }
}
