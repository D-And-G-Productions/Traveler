#include "JourneyService.hpp"
#include "persistence/JourneyStore.hpp"

Journey JourneyService::getJourney(const int64_t journeyId, const int64_t userId) const
{
  DBPool::Lease lease{pool->acquire()};
  pqxx::read_transaction transaction{lease.connection()};
  JourneyStore journeyStore{transaction};
  return journeyStore.selectJourney(journeyId, userId);
}

std::vector<Journey> JourneyService::getJourneys(const int64_t userId) const
{
  DBPool::Lease lease = pool->acquire();
  pqxx::read_transaction transaction{lease.connection()};
  JourneyStore journeyStore{transaction};
  return journeyStore.selectJourneysByUserId(userId);
}

Journey JourneyService::addJourney(const JourneyCreate &jc)
{
  DBPool::Lease lease = pool->acquire();
  pqxx::work transaction{lease.connection()};
  JourneyStore journeyStore{transaction};
  Journey journey = journeyStore.insertJourney(jc);
  transaction.commit();
  return journey;
}

void JourneyService::deleteJourney(const int64_t journeyId, const int64_t userId)
{
  DBPool::Lease lease{pool->acquire()};
  pqxx::work transaction{lease.connection()};
  JourneyStore journeyStore{transaction};
  journeyStore.deleteJourney(journeyId, userId);
  transaction.commit();
}

Journey JourneyService::updateJourney(
    const int64_t journeyId,
    const int64_t userId,
    const JourneyUpdate &ju
)
{
  DBPool::Lease lease = pool->acquire();
  pqxx::work transaction{lease.connection()};
  JourneyStore journeyStore{transaction};
  Journey journey = journeyStore.updateJourney(journeyId, userId, ju);
  transaction.commit();
  return journey;
}
