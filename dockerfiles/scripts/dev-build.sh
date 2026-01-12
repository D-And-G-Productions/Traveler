#!/usr/bin/env bash
set -euo pipefail

ENV_FILE="${1:-.env.dev-info}"

# Export variables from env file into this shell
set -a
source "${ENV_FILE}"
set +a

: "${DEV_BASE_IMAGE:=traveler/dev-base:local}"
: "${COMPOSE_FILE:=docker-compose.dev.yml}"

echo "Building dev base: ${DEV_BASE_IMAGE}"
docker build \
  -f Dockerfile.dev-base \
  -t "${DEV_BASE_IMAGE}" \
  .

echo "Starting dev container using ${DEV_DOCKERFILE:-Dockerfile.dev-base}"
docker compose \
  --env-file "${ENV_FILE}" \
  -f "${COMPOSE_FILE}" \
  up --build -d
