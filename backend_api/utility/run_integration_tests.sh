#!/bin/bash

# NOTE: add the firebase key and token in the script as such
# FIREBASE_API_KEY="AIza..." TEST_ID_TOKEN="ey..." ./utility/run_integration_tests.sh

#TODO: Once integrated with new code base use env files to automatically run the script
# with token, and firebase api key

set -e

SCRIPT_DIRECTORY="$(dirname "$0")"
PARENT_DIRECTORY="$SCRIPT_DIRECTORY/.."
cd "$PARENT_DIRECTORY"

# 1. Ensure latest code is built
./utility/build.sh

# 2. Check for required variables
if [ -z "$FIREBASE_API_KEY" ] || [ -z "$TEST_ID_TOKEN" ]; then
    echo "Error: Missing environment variables."
    echo "Usage: FIREBASE_API_KEY=... TEST_ID_TOKEN=... ./utility/run_integration_tests.sh"
    exit 1
fi

# 3. Run the Integration Executable directly
echo "Running Integration Tests..."
./build/tests/IntegrationTests
