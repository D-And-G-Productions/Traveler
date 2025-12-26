set -e

SCRIPT_DIRECTORY="$(dirname "$0")"
PARENT_DIRECTORY="$SCRIPT_DIRECTORY/.."
cd "$PARENT_DIRECTORY"

npx @redocly/cli@latest build-docs traveler-api.yaml -o traveler-api-docs.html
