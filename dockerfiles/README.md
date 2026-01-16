# Dockerfiles

## Setup

### Container Setup

1. Copy `Docker.dev-example` to `Docker.dev-your-name` e.g. `Docker.dev-gbaudelaire`.
2. Copy `.env.dev-info.example` to `.env.dev-info`.
    1. Set `DEV_DOCKERFILE=Dockerfile.dev-your-name` e.g. `DEV_DOCKERFILE=Dockerfile.dev-gbaudelaire`.

### Database Setup

Once the container is setup, setup the database by:

1. `cd` into `migrations/`.
2. Run `scripts/migrate_up.sh`

### Verify

Run tests to verify everything is working.

1. `cd` into `backend_api/`.
2. Run `scripts/run_tests.sh`

## Running the Container

Run the scripts from the `dockerfiles/` directory.

**Build Container** - `scripts/dev-build.sh`

**Run Container** - `scripts/dev-run.sh`

**Stop Container** - `scripts/dev-stop.sh`
