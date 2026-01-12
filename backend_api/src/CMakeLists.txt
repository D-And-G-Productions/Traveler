ADD_LIBRARY(Core STATIC
  ./core/application/JourneyService.cpp
  ./core/application/UserService.cpp
  ./core/persistence/UserStore.cpp
  ./core/persistence/JourneyStore.cpp
  ./core/persistence/Queries.cpp
)

TARGET_INCLUDE_DIRECTORIES(Core PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/core
)

TARGET_LINK_LIBRARIES(Core
  PUBLIC
    pqxx
    pq
)

ADD_LIBRARY(WebserverLib STATIC
  ./webapp/api/Mappings.cpp
  ./webapp/app/Server.cpp
  ./webapp/http/middleware/AuthMiddleware.cpp
  ./webapp/http/controllers/JourneyController.cpp
  ./webapp/http/controllers/HealthController.cpp
  ./webapp/http/controllers/MeController.cpp
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
