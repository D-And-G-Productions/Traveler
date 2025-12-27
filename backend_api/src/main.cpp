#include "app/App.h"

int main() {
  auto app = buildApp();
  app.port(18080).multithreaded().run();
}
