CMAKE_MINIMUM_REQUIRED(VERSION 3.31)
PROJECT(Webserver LANGUAGES CXX)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

INCLUDE(FetchContent)

# ----------------------------
# Options
# ----------------------------
OPTION(WEBSERVER_BUILD_TESTS "Build tests" ON)

# ----------------------------
# Dependencies
# ----------------------------
SET(GOOGLE_TEST_URL
    https://github.com/google/googletest/archive/9156d4caac880b513264ecbe0aa4746a3fead3d7.zip)
SET(CPR_URL https://github.com/libcpr/cpr.git)

FIND_PACKAGE(Crow CONFIG REQUIRED)
FIND_PACKAGE(OpenSSL REQUIRED)
FIND_PACKAGE(ZLIB REQUIRED)

INCLUDE(FetchContent)

FETCHCONTENT_DECLARE(googletest URL ${GOOGLE_TEST_URL})
FETCHCONTENT_MAKEAVAILABLE(googletest)

FETCHCONTENT_DECLARE(cpr GIT_REPOSITORY ${CPR_URL} GIT_TAG 1.11.0)
FETCHCONTENT_MAKEAVAILABLE(cpr)

# ----------------------------
# Subprojects
# ----------------------------
ADD_SUBDIRECTORY(src)

IF(WEBSERVER_BUILD_TESTS)
  ENABLE_TESTING()
  ADD_SUBDIRECTORY(./tests)
ENDIF()

