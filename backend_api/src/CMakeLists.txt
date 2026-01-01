ADD_LIBRARY(WebserverLib
  ./http/controllers/HealthController.hpp
  ./http/middleware/AuthMiddleware.cpp
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
