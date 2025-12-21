#include "../dto/Travel.hpp"
#include <oatpp/json/ObjectMapper.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

#include OATPP_CODEGEN_END(DTO)

#include OATPP_CODEGEN_BEGIN(ApiController)

class TravelController : public oatpp::web::server::api::ApiController {
public:
  explicit TravelController(
      const std::shared_ptr<oatpp::data::mapping::ObjectMapper> &objectMapper)
      : oatpp::web::server::api::ApiController(objectMapper) {}

  ENDPOINT("GET", "/travel", getTravel, QUERY(oatpp::String, origin),
           QUERY(oatpp::String, destination),
           QUERY(oatpp::String, arrival_time)) {

    OATPP_ASSERT_HTTP(origin && origin->size() > 0, Status::CODE_400,
                      "Missing query param: origin");
    OATPP_ASSERT_HTTP(destination && destination->size() > 0, Status::CODE_400,
                      "Missing query param: destination");
    OATPP_ASSERT_HTTP(arrival_time && arrival_time->size() > 0,
                      Status::CODE_400, "Missing query param: arrival_time");

    auto dto = TravelAckDto::createShared();
    dto->origin = origin;
    dto->destination = destination;
    dto->arrival_time = arrival_time;
    dto->message = "Received request: origin=" + origin +
                   ", destination=" + destination +
                   ", arrival_time=" + arrival_time;

    return createDtoResponse(Status::CODE_200, dto);
  }
};

#include OATPP_CODEGEN_END(ApiController)
