#pragma once

namespace Queries
{
extern const char selectUserBySubject[];
extern const char selectUserById[];
extern const char insertUserBySubQuery[];

extern const char selectJourneysByUserIdQuery[];
extern const char insertLocationQuery[];
extern const char insertJourney[];

extern const char selectJourneyById[];
extern const char selectJourneyByIdAndUserId[];

extern const char deleteJourneyByIdAndUserIdQuery[];
extern const char updateJourneyByIdAndUserIdQuery[];
} // namespace Queries
