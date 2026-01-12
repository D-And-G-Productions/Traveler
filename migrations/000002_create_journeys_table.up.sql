BEGIN;

CREATE TABLE locations (
  id BIGSERIAL PRIMARY KEY,
  label TEXT,
  latitude DOUBLE PRECISION NOT NULL,
  longitude DOUBLE PRECISION NOT NULL
);

CREATE TYPE transportation_mode AS ENUM ('DRIVING', 'TRANSIT', 'WALKING', 'CYCLING');

CREATE TABLE journeys (
  id BIGSERIAL PRIMARY KEY,
  user_id BIGINT NOT NULL REFERENCES users(id) ON DELETE CASCADE,
  source_location_id BIGINT NOT NULL REFERENCES locations(id) ON DELETE CASCADE,
  destination_location_id BIGINT NOT NULL REFERENCES locations(id) ON DELETE CASCADE,
  arrival_time TIMESTAMPTZ NOT NULL,
  mode transportation_mode NOT NULL,
  created_at TIMESTAMPTZ NOT NULL DEFAULT now(),
  updated_at TIMESTAMPTZ NOT NULL DEFAULT now()
);

CREATE INDEX journeys_user_id_index ON journeys(user_id);

CREATE TRIGGER journeys_set_updated_at
BEFORE UPDATE ON journeys
FOR EACH ROW EXECUTE FUNCTION set_updated_at();

COMMIT;
