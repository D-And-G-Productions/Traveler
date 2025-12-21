#include <oatpp/json/ObjectMapper.hpp>
#include <oatpp/macro/codegen.hpp>
#include <oatpp/macro/component.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

class TravelAckDto : public oatpp::DTO {
  DTO_INIT(TravelAckDto, DTO)

  DTO_FIELD(oatpp::String, origin);
  DTO_FIELD(oatpp::String, destination);
  DTO_FIELD(oatpp::String, arrival_time); // expected ISO-8601 string
  DTO_FIELD(oatpp::String, message);
};
