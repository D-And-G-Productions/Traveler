# 1. Base stage: Install Node and basic tools
FROM node:20-slim AS base
WORKDIR /workspace
# Install global CLI tools (like expo-cli if using Expo)
RUN npm install -g expo-cli

# 2. Development stage (The 'target' in your compose file)
FROM base AS dev
# Ensure we have git for some npm packages
RUN apt-get update && apt-get install -y git zsh
# Set shell to zsh as requested in your compose
ENV SHELL=/bin/zsh

# Copy package files first to cache layers
COPY package*.json ./
RUN npm install

# Copy the rest of the code
COPY . .

# Start the dev server
CMD ["npx", "expo", "start", "--web"]
