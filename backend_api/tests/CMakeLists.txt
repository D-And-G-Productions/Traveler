INCLUDE(GoogleTest)

ADD_LIBRARY(TestsSupport INTERFACE)

TARGET_INCLUDE_DIRECTORIES(TestsSupport INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}
)

TARGET_LINK_LIBRARIES(TestsSupport INTERFACE
  WebserverLib
  cpr::cpr
  GTest::gtest_main
)

ADD_EXECUTABLE(RoutesTests
  ./test_routes/TestJourneyGet.cpp
  ./test_routes/TestJourneyIdGet.cpp
  ./test_routes/TestJourneyPost.cpp
  ./test_routes/TestJourneyIdPut.cpp
  ./test_routes/TestHealthGet.cpp
  ./test_routes/TestMeGet.cpp
)

ADD_EXECUTABLE(MiddlewareTests
  ./test_middleware/TestAuthMiddleware.cpp
)

TARGET_LINK_LIBRARIES(RoutesTests PRIVATE TestsSupport)
TARGET_LINK_LIBRARIES(MiddlewareTests PRIVATE TestsSupport)

GTEST_DISCOVER_TESTS(RoutesTests)
# GTEST_DISCOVER_TESTS(MiddlewareTests)
