#!/bin/bash

set -e

SCRIPT_DIRECTORY="$(dirname "$0")"
PARENT_DIRECTORY="$SCRIPT_DIRECTORY/.."
cd "$PARENT_DIRECTORY"

migrate -database "$DATABASE_URL?sslmode=disable" -path . up
