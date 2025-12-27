INCLUDE(GoogleTest)

ADD_LIBRARY(tests_support STATIC
  ./support/CrowTestServer.cpp
  ./support/CPRRouteFixture.cpp
)

TARGET_INCLUDE_DIRECTORIES(tests_support PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}
)

TARGET_LINK_LIBRARIES(tests_support PUBLIC
  WebserverLib
  cpr::cpr
  GTest::gtest_main
)

ADD_EXECUTABLE(journey_routes_tests
  ./routes/JourneyFlowTest.cpp
  ./routes/JourneyGetTest.cpp
  ./routes/JourneyPostTest.cpp
  ./routes/JourneyIdGetTest.cpp
  ./routes/JourneyIdPutTest.cpp
)

TARGET_LINK_LIBRARIES(journey_routes_tests PRIVATE
  tests_support
)
GTEST_DISCOVER_TESTS(journey_routes_tests)
