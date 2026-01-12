BEGIN;

DROP TRIGGER IF EXISTS journeys_set_updated_at ON journeys;

DROP TABLE IF EXISTS journeys;
DROP TYPE IF EXISTS transportation_mode;
DROP TABLE IF EXISTS locations;

COMMIT;
