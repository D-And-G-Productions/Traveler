#!/usr/bin/bash

set -euo pipefail

SCRIPT_DIRECTORY="$(dirname "$0")"
PARENT_DIRECTORY="$SCRIPT_DIRECTORY/../.."
cd "$PARENT_DIRECTORY"

docker compose stop dev >/dev/null 2>&1 || true
docker compose build dev
