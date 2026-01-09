#!/bin/bash
cd "$(dirname "$0")/.."

# Check if the 'dev' service container is already running
if [ -z "$(docker compose ps -q dev)" ]; then
    echo "Container not running. Starting..."
    docker compose up -d --build dev
else
    echo "Container already running. Connecting..."
fi

# Open a new shell session without restarting the container
docker compose exec dev zsh
