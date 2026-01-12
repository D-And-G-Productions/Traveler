#pragma once

#include "api/dto/JourneyCreateRequestContract.hpp"
#include "api/dto/JourneyResponseContract.hpp"
#include "api/dto/JourneyUpdateRequest.hpp"
#include "api/dto/UserReponseContract.hpp"
#include "domain/Journey.hpp"
#include "domain/JourneyCreate.hpp"
#include "domain/JourneyUpdate.hpp"
#include "domain/Location.hpp"
#include "domain/LocationCreate.hpp"
#include "domain/User.hpp"
#include <cstdint>

namespace ContractMappings
{
UserResponseContract toUserResponseContract(const User &u);

LocationCreate toLocationCreate(const LocationCreateRequestContract &l);

JourneyCreate toJourneyCreate(const int64_t userId, const JourneyCreateRequestContract &j);

LocationResponseContract toLocationResponseContract(const Location &l);

JourneyResponseContract toJourneyResponseContract(const Journey &j);

JourneyUpdate toJourneyUpdate(JourneyUpdateRequestContract &j);
} // namespace ContractMappings
