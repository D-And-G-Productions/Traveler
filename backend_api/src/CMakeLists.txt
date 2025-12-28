ADD_LIBRARY(WebserverLib
  ./app/App.h
  ./app/App.cpp
  ./domain/Journey.h
  ./domain/Journey.h
  ./domain/JourneyRecord.h
  ./domain/Location.h
  ./domain/Mode.h
  ./domain/WithId.h
  ./http/controllers/JourneyController.h
  ./http/controllers/JourneyController.cpp
  ./http/controllers/HealthController.hpp
  ./http/controllers/HealthController.cpp
  ./json/JourneyJson.h
  ./json/JourneyJson.cpp
  ./repository/JourneyRepository.h
  ./repository/MockJourneyRepository.h
)

# Make includes clean from other targets (tests, main)
TARGET_INCLUDE_DIRECTORIES(WebserverLib
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

TARGET_LINK_LIBRARIES(WebserverLib
  PUBLIC
    Crow::Crow
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
