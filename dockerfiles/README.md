# Dockerfiles

## Setup

1. Copy `Docker.dev-example` to `Docker.dev-your-name` e.g. `Docker.dev-gbaudelaire`.
2. Copy `.env.dev-info.example` to `.env.dev-info`.
    1. Set `DEV_DOCKERFILE=Dockerfile.dev-your-name` e.g. `DEV_DOCKERFILE=Dockerfile.dev-gbaudelaire`.

## Running the Container

Run the scripts from the `dockerfiles/` directory.

**Build Container** - `scripts/dev-build.sh`

**Run Container** - `scripts/dev-run.sh`

**Stop Container** - `scripts/dev-stop.sh`
