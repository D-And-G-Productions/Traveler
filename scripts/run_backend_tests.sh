#!/bin/bash

set -e

SCRIPT_DIRECTORY="$(dirname "$0")"
PARENT_DIRECTORY="$SCRIPT_DIRECTORY/.."
cd "$PARENT_DIRECTORY"
cd backend_api
docker build --target test -t traveler-backend-tests .
docker run --rm -e DATABASE_URL="$DATABASE_URL" traveler-backend-tests
