#!/bin/bash

set -e

SCRIPT_DIRECTORY="$(dirname "$0")"
PARENT_DIRECTORY="$SCRIPT_DIRECTORY/.."
cd "$PARENT_DIRECTORY"

find src tests \( -name '*.c' -o -name '*.cc' -o -name '*.cpp' -o -name '*.cxx' -o -name '*.h' -o -name '*.hh' -o -name '*.hpp' -o -name '*.hxx' \) \
  -print0 | xargs -0 clang-format -i -style=file
