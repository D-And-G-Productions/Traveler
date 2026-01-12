#include "persistence/Queries.hpp"

namespace Queries
{

const char selectUserBySubject[] = R"SQL(
      SELECT id, subject, created_at, updated_at, name, telephone
      FROM users 
      WHERE subject = $1
    )SQL";

const char selectUserById[] = R"SQL(
      SELECT id, subject, created_at, updated_at, name, telephone
      FROM users
      WHERE id = $1
    )SQL";

const char insertUserBySubQuery[] = R"SQL(
      INSERT INTO users (subject)
      VALUES ($1)
      RETURNING id, subject, created_at, updated_at, name, telephone
    )SQL";

const char selectJourneysByUserIdQuery[] = R"SQL(
      SELECT
        j.id,
        j.user_id,
        j.arrival_time,
        j.mode,
        j.created_at,
        j.updated_at,

        src.id        AS source_location_id,
        src.latitude  AS source_latitude,
        src.longitude AS source_longitude,

        dst.id        AS destination_location_id,
        dst.latitude  AS destination_latitude,
        dst.longitude AS destination_longitude

      FROM journeys j
      JOIN locations src ON src.id = j.source_location_id
      JOIN locations dst ON dst.id = j.destination_location_id
      WHERE j.user_id = $1
      ORDER BY j.id
    )SQL";

const char insertLocationQuery[] = R"SQL(
      INSERT INTO locations (latitude, longitude)
      VALUES ($1, $2)
      RETURNING id
  )SQL";

const char insertJourney[] = R"SQL(
      WITH
        src AS (
          INSERT INTO locations (label, latitude, longitude)
          VALUES ($4, $5, $6)
          RETURNING id, latitude, longitude
        ),
        dst AS (
          INSERT INTO locations (label, latitude, longitude)
          VALUES ($7, $8, $9)
          RETURNING id, latitude, longitude
        ),
        j AS (
          INSERT INTO journeys (user_id, arrival_time, mode, source_location_id, destination_location_id)
          SELECT $1, $2, $3::transportation_mode, src.id, dst.id
          FROM src, dst
          RETURNING id, user_id, arrival_time, mode, created_at, updated_at
        )
      SELECT
        j.id,
        j.user_id,
        j.arrival_time,
        j.mode,
        j.created_at,
        j.updated_at,

        src.id        AS source_location_id,
        src.latitude  AS source_latitude,
        src.longitude AS source_longitude,

        dst.id        AS destination_location_id,
        dst.latitude  AS destination_latitude,
        dst.longitude AS destination_longitude
      FROM j, src, dst
  )SQL";

const char selectJourneyById[] = R"SQL(
      SELECT
        j.id,
        j.user_id,
        j.arrival_time,
        j.mode,
        j.created_at,
        j.updated_at,

        src.id        AS source_location_id,
        src.latitude  AS source_latitude,
        src.longitude AS source_longitude,

        dst.id        AS destination_location_id,
        dst.latitude  AS destination_latitude,
        dst.longitude AS destination_longitude

      FROM journeys j
      JOIN locations src ON src.id = j.source_location_id
      JOIN locations dst ON dst.id = j.destination_location_id
      WHERE j.id = $1
      ORDER BY j.id
  )SQL";

const char selectJourneyByIdAndUserId[] = R"SQL(
      SELECT
        j.id,
        j.user_id,
        j.arrival_time,
        j.mode,
        j.created_at,
        j.updated_at,

        src.id        AS source_location_id,
        src.latitude  AS source_latitude,
        src.longitude AS source_longitude,

        dst.id        AS destination_location_id,
        dst.latitude  AS destination_latitude,
        dst.longitude AS destination_longitude

      FROM journeys j
      JOIN locations src ON src.id = j.source_location_id
      JOIN locations dst ON dst.id = j.destination_location_id
      WHERE j.id = $1 AND j.user_id = $2
      ORDER BY j.id
  )SQL";

const char deleteJourneyByIdAndUserIdQuery[] = R"SQL(
      DELETE FROM journeys
      WHERE id = $1 AND user_id = $2
      RETURNING id;
  )SQL";

const char updateJourneyByIdAndUserIdQuery[] = R"SQL(
      WITH target AS (
        SELECT id, source_location_id, destination_location_id
        FROM journeys
        WHERE id = $1 AND user_id = $2
      ),
      updated_src AS (
        UPDATE locations l
        SET
          latitude  = COALESCE($3, l.latitude),
          longitude = COALESCE($4, l.longitude)
        FROM target t
        WHERE l.id = t.source_location_id
        RETURNING l.id
      ),
      updated_dst AS (
        UPDATE locations l
        SET
          latitude  = COALESCE($5, l.latitude),
          longitude = COALESCE($6, l.longitude)
        FROM target t
        WHERE l.id = t.destination_location_id
        RETURNING l.id
      ),
      updated_j AS (
        UPDATE journeys j
        SET
          arrival_time = COALESCE($7, j.arrival_time),
          mode         = COALESCE($8::transportation_mode, j.mode)
        FROM target t
        WHERE j.id = t.id
        RETURNING
          j.id,
          j.user_id,
          j.arrival_time,
          j.mode,
          j.created_at,
          j.updated_at,
          j.source_location_id,
          j.destination_location_id
      )
      SELECT
        j.id,
        j.user_id,
        j.arrival_time,
        j.mode,
        j.created_at,
        j.updated_at,

        src.id        AS source_location_id,
        src.latitude  AS source_latitude,
        src.longitude AS source_longitude,

        dst.id        AS destination_location_id,
        dst.latitude  AS destination_latitude,
        dst.longitude AS destination_longitude
      FROM updated_j j
      JOIN locations src ON src.id = j.source_location_id
      JOIN locations dst ON dst.id = j.destination_location_id;
  )SQL";

} // namespace Queries
