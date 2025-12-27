#include "support/CPRRouteFixture.h"
#include <crow.h>

class Foo : public CPRRouteFixture {
public:
  std::string journeyUrl() { return server->baseUrl() + "/journey"; }
};

TEST_F(Foo, PostInvalidJsonReturns400) {
  cpr::Response response = postJson(journeyUrl(), "{ this is not json }");
  EXPECT_EQ(response.status_code, crow::BAD_REQUEST);
}
