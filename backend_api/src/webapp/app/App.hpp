#include "http/middleware/AuthMiddleware.hpp"
#include <crow/app.h>

using TravelerApp = crow::App<AuthMiddleware>;
