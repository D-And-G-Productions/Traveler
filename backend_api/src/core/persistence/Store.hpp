#pragma once

#include <pqxx/pqxx>

class Store
{
public:
  Store(pqxx::transaction_base &t) : transaction(t) {}

protected:
  pqxx::transaction_base &transaction;
};
