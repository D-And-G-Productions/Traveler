#!/bin/bash

set -e

SCRIPT_DIRECTORY="$(dirname "$0")"
PARENT_DIRECTORY="$SCRIPT_DIRECTORY/.."
cd "$PARENT_DIRECTORY"

cmake \
  -S . \
  -B build \
  -G Ninja \
  -DWEBSERVER_BUILD_TESTS=ON
cmake --build build
cd build
ctest --output-on-failure
