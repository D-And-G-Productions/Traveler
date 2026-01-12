#!/usr/bin/env bash
set -euo pipefail

ENV_FILE="${1:-.env.dev-info}"

# Export variables from env file into this shell
set -a
source "${ENV_FILE}"
set +a

: "${COMPOSE_FILE:=docker-compose.dev.yml}"

docker compose \
  --env-file "${ENV_FILE}" \
  -f "${COMPOSE_FILE}" \
  start

docker compose \
  --env-file "${ENV_FILE}" \
  -f "${COMPOSE_FILE}" \
  exec -it dev zsh
