# Setup Project

CMAKE_MINIMUM_REQUIRED(VERSION 4.0)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(projectName TravelerWebserver)

PROJECT(${projectName})

FIND_PACKAGE(oatpp REQUIRED)

SET(CMAKE_CXX_STANDARD 17)

# Setup Core Library

ADD_LIBRARY(${projectName}Lib
  src/AppComponent.hpp
  src/controller/TravelController.hpp
  src/dto/Travel.hpp
  # src/controller/MyController.cpp
  # src/controller/MyController.hpp
  # src/dto/DTOs.hpp
)

TARGET_LINK_LIBRARIES(${projectName}Lib
  PUBLIC
    oatpp::oatpp
    oatpp::oatpp-test
)

TARGET_INCLUDE_DIRECTORIES(${projectName}Lib PUBLIC src)

# Setup Application

ADD_EXECUTABLE(${projectName}Exe
  src/App.cpp
)

TARGET_LINK_LIBRARIES(${projectName}Exe ${projectName}Lib)

ADD_DEPENDENCIES(${projectName}Exe ${projectName}Lib)

# Setup Tests

# ADD_EXECUTABLE(${projectName}Test
#         test/tests.cpp
#         test/app/TestComponent.hpp
#         test/app/MyApiTestClient.hpp
#         test/MyControllerTest.cpp
#         test/MyControllerTest.hpp
# )
#
# TARGET_LINK_LIBRARIES(${projectName}Test ${projectName}Lib)
# ADD_DEPENDENCIES(${ProjectName}-test ${ProjectName}-lib)
#
# SET_TARGET_PROPERTIES(
#   ${ProjectName}-lib ${ProjectName}-exe ${ProjectName}-test
#     PROPERTIES
#       CXX_STANDARD 17
#       CXX_EXTENSIONS OFF
#       CXX_STANDARD_REQUIRED ON
# )

# ENABLE_TESTING()
# ADD_TEST(project-tests ${ProjectName}-test)
