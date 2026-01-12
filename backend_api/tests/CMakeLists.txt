INCLUDE(GoogleTest)
ADD_LIBRARY(TestsSupport STATIC
  ./support/fixtures/MeRoutes.cpp
  ./support/TestServer.cpp
  ./support/fixtures/JourneyRoutes.cpp
)

TARGET_INCLUDE_DIRECTORIES(TestsSupport PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/support
)

TARGET_LINK_LIBRARIES(TestsSupport PUBLIC
  WebserverLib
  cpr::cpr
  GTest::gtest_main
)

ADD_EXECUTABLE(TestRoutes
  ./test_webapp/TestJourneyRoutes.cpp
  ./test_webapp/TestHealthRoutes.cpp
  ./test_webapp/TestMeRoutes.cpp
)
TARGET_LINK_LIBRARIES(TestRoutes PRIVATE TestsSupport)
GTEST_DISCOVER_TESTS(TestRoutes)

ADD_EXECUTABLE(AuthMiddlewareTests
  ./test_webapp/TestAuthMiddleware.cpp
)
TARGET_LINK_LIBRARIES(AuthMiddlewareTests PRIVATE TestsSupport)
GTEST_DISCOVER_TESTS(AuthMiddlewareTests)

ADD_EXECUTABLE(PersistenceTests
  ./test_persistence/LibpqxxExplorationTests.cpp
)
TARGET_LINK_LIBRARIES(PersistenceTests PRIVATE TestsSupport)
GTEST_DISCOVER_TESTS(PersistenceTests)
