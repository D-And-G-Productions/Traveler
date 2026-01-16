#include "fixtures/UserStoreFixture.hpp"
#include "persistence/StoreErrors.hpp"
#include <cstdint>

TEST_F(UserStoreFixture, InsertUserBySubject)
{
  const std::string USER_SUBJECT = "SUBJECT_STRING";
  User user = userStore->insertUser(USER_SUBJECT);
  EXPECT_EQ(user.subject, USER_SUBJECT);
}

TEST_F(UserStoreFixture, ExistingSubjectThrows)
{
  auto statement = [&]() -> void { userStore->insertUser(existingUser.subject); };
  EXPECT_THROW(statement(), UserAlreadyExists);
}

TEST_F(UserStoreFixture, UserBySubject)
{
  User selectedUser = userStore->selectUser(existingUser.subject);
  EXPECT_EQ(selectedUser, existingUser);
}

TEST_F(UserStoreFixture, NonExistentSubjectThrows)
{
  auto statement = [&]() -> void { userStore->selectUser("NON_EXISTENT_USER_SUBJECT"); };
  EXPECT_THROW(statement(), UserDoesNotExist);
}

TEST_F(UserStoreFixture, UserById)
{
  User selectedUser = userStore->selectUser(existingUser.id);
  EXPECT_EQ(selectedUser, existingUser);
}

TEST_F(UserStoreFixture, NonExistentIdThrows)
{
  int64_t nonExistentId = existingUser.id + 1;
  auto statement = [&]() -> void { userStore->selectUser(nonExistentId); };
  EXPECT_THROW(statement(), UserDoesNotExist);
}
