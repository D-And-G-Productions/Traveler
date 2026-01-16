#include "UserStore.hpp"
#include "Queries.hpp"
#include "persistence/DBPool.hpp"
#include "persistence/StoreErrors.hpp"
#include <cstdint>
#include <format>
#include <optional>
#include <string_view>

namespace
{
std::optional<std::string> parseOptionalNullString(const pqxx::row row, const std::string &field)
{
  pqxx::field f = row[field];
  return f.is_null() ? std::nullopt : std::make_optional(f.as<std::string>());
}

User userFromRow(pqxx::row &r)
{
  return {
      .id = r["id"].as<std::int64_t>(),
      .subject = r["subject"].as<std::string>(),
      .createdAt = r["created_at"].as<std::string>(),
      .updatedAt = r["updated_at"].as<std::string>(),
      .name = parseOptionalNullString(r, "name"),
      .telephone = parseOptionalNullString(r, "telephone"),
  };
}

void requireUserIsFound(const pqxx::result &result, const std::string_view subject)
{
  if (result.empty())
  {
    const std::string message = std::format("User with subject '{}' not found", subject);
    throw UserDoesNotExist(message);
  }
}

void requireUserIsFound(const pqxx::result &result, const std::int64_t id)
{
  if (result.empty())
  {
    const std::string message = std::format("User with id '{}' not found", id);
    throw UserDoesNotExist(message);
  }
}
} // namespace

User UserStore::selectUser(const std::string_view subject) const
{
  pqxx::params parameters = {subject};
  const pqxx::result result = transaction.exec(Queries::selectUserBySubject, parameters);
  requireUserIsFound(result, subject);
  pqxx::row row = result.one_row();
  return userFromRow(row);
}

User UserStore::selectUser(const std::int64_t id) const
{
  pqxx::params parameters = {id};
  const pqxx::result result = transaction.exec(Queries::selectUserById, parameters);
  requireUserIsFound(result, id);
  pqxx::row row = result.one_row();
  return userFromRow(row);
}

User UserStore::insertUser(const std::string_view subject)
{
  const pqxx::result result = executeInsertUser(subject);
  pqxx::row row = result.one_row();
  return userFromRow(row);
}

pqxx::result UserStore::executeInsertUser(const std::string_view subject)
{
  try
  {
    pqxx::params parameters = {subject};
    return transaction.exec(Queries::insertUserBySubQuery, parameters);
  }
  catch (pqxx::unique_violation &)
  {
    const std::string message = std::format("User with subject '{}' already exists", subject);
    throw UserAlreadyExists(message);
  }
}
