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

ADD_EXECUTABLE(TestJourneyGet ./test_routes/TestJourneyGet.cpp)
TARGET_LINK_LIBRARIES(TestJourneyGet PRIVATE TestsSupport)
GTEST_DISCOVER_TESTS(TestJourneyGet)

ADD_EXECUTABLE(TestJourneyIdGet ./test_routes/TestJourneyIdGet.cpp)
TARGET_LINK_LIBRARIES(TestJourneyIdGet PRIVATE TestsSupport)
GTEST_DISCOVER_TESTS(TestJourneyIdGet)

ADD_EXECUTABLE(TestJourneyPost ./test_routes/TestJourneyPost.cpp)
TARGET_LINK_LIBRARIES(TestJourneyPost PRIVATE TestsSupport)
GTEST_DISCOVER_TESTS(TestJourneyPost)

ADD_EXECUTABLE(TestJourneyIdPut ./test_routes/TestJourneyIdPut.cpp)
TARGET_LINK_LIBRARIES(TestJourneyIdPut PRIVATE TestsSupport)
GTEST_DISCOVER_TESTS(TestJourneyIdPut)

ADD_EXECUTABLE(TestJourneyIdDelete ./test_routes/TestJourneyIdDelete.cpp)
TARGET_LINK_LIBRARIES(TestJourneyIdDelete PRIVATE TestsSupport)
GTEST_DISCOVER_TESTS(TestJourneyIdDelete)

ADD_EXECUTABLE(TestHealthGet ./test_routes/TestHealthGet.cpp)
TARGET_LINK_LIBRARIES(TestHealthGet PRIVATE TestsSupport)
GTEST_DISCOVER_TESTS(TestHealthGet)

ADD_EXECUTABLE(TestMeGet ./test_routes/TestMeGet.cpp)
TARGET_LINK_LIBRARIES(TestMeGet PRIVATE TestsSupport)
GTEST_DISCOVER_TESTS(TestMeGet)

ADD_EXECUTABLE(AuthMiddlewareTests ./test_middleware/TestAuthMiddleware.cpp)
TARGET_LINK_LIBRARIES(AuthMiddlewareTests PRIVATE TestsSupport)
GTEST_DISCOVER_TESTS(AuthMiddlewareTests)

ADD_EXECUTABLE(IntegrationTests ./test_integration/FirebaseAuthTest.cpp) 

TARGET_LINK_LIBRARIES(IntegrationTests 
    PRIVATE 
    WebserverLib # Your API code
    cpr::cpr     # Network library
)
