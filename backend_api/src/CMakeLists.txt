ADD_LIBRARY(Core STATIC
  ./core/application/JourneyService.cpp
)

TARGET_INCLUDE_DIRECTORIES(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/core)

ADD_LIBRARY(WebserverLib STATIC
  ./webapp/http/controllers/HealthController.hpp
  ./webapp/http/middleware/AuthMiddleware.cpp
  ./webapp/http/controllers/JourneyController.cpp
)

TARGET_INCLUDE_DIRECTORIES(WebserverLib
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/webapp
)

TARGET_LINK_LIBRARIES(WebserverLib
  PUBLIC
    Crow::Crow
    Core
)

ADD_EXECUTABLE(WebserverMain
  main.cpp
)

TARGET_LINK_LIBRARIES(WebserverMain
  PRIVATE
    WebserverLib
    OpenSSL::SSL
    OpenSSL::Crypto
    ZLIB::ZLIB
)
