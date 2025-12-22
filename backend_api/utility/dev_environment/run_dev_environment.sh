#!/usr/bin/bash

set -euo pipefail

SCRIPT_DIRECTORY="$(dirname "$0")"
PARENT_DIRECTORY="$SCRIPT_DIRECTORY/../.."
cd "$PARENT_DIRECTORY"

cleanup() {
  docker compose stop dev >/dev/null 2>&1 || true
}

trap cleanup EXIT INT TERM
docker compose up -d dev
docker compose exec -it dev bash
