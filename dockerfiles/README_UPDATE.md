# Development containers

## Overview
This repository uses a shared development base image and optional personal dev images to ensure a consistent toolchain across contributors. The application can be run locally via Docker Compose either against a local Postgres container or against RDS.

## Files and roles

### Dev images (repo root)
- `Dockerfile.dev-base`  
  Team-standard developer toolchain image (compilers, CMake/Ninja, libraries). Built first and reused by all developers.

- `Dockerfile.dev-<name>` (e.g. `Dockerfile.dev-gbaudelaire`)  
  Personal dev image that extends `dev-base` and adds individual preferences. Do not `COPY ~/...` in these Dockerfiles; mount dotfiles via volumes.

- `.env.dev-info` (gitignored) and `.env.dev-info.example`  
  Stores per-developer settings such as `DEV_DOCKERFILE` used by the dev script and Compose.

### Dev container runtime (repo root)
- `docker-compose.dev.yml`  
  Defines the interactive development container. Mounts the repository into `/workspace` and mounts editor configuration via volumes.

- `scripts/dev-up.sh`  
  Loads `.env.dev-info`, builds `dev-base`, then starts the dev container (typically detached) and optionally opens a shell.

### Application stack (repo root)
- `docker-compose.yml`  
  Base stack (API and any always-on services).

- `docker-compose.localdb.yml`  
  Local override that adds a Postgres container and runs migrations locally, and points the API at `db:5432`.

- `migrations/`  
  SQL migrations. Applied in CI as a dedicated step before deployment. Optionally run locally via `docker-compose.localdb.yml`.

- `postgres-cluster/`  
  Optional location for Postgres-related local infra assets (compose fragments, init scripts, helper scripts, documentation). Database data should be stored in Docker volumes, not in the repo.

### Backend tests (backend_api)
- `backend_api/Dockerfile`  
  Multi-stage Dockerfile that builds the backend in one stage and runs tests in another stage.

- `backend_api/utility/run_tests.sh`  
  Existing backend test runner used by the test image.

- `scripts/run_backend_tests.sh`  
  Repo-root convenience wrapper for running backend tests.

## Common commands

### Start a dev container
- Configure `.env.dev-info` (copy from `.env.dev-info.example`).
- Run:
  - `./scripts/dev-up.sh`

### Run the application locally with a local database
- `docker compose -f docker-compose.yml -f docker-compose.localdb.yml up --build`

### Run the application locally against RDS
- Set `DATABASE_URL` to your RDS endpoint.
- `docker compose -f docker-compose.yml up --build`

### Run backend tests in Docker
- `./scripts/run_backend_tests.sh`
