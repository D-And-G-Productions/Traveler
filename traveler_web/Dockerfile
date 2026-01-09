# 1. Base stage
FROM node:20-slim AS base
WORKDIR /workspace

# 2. Development stage
FROM base AS dev

# Install dependencies for Neovim and Zsh
RUN apt-get update && apt-get install -y --no-install-recommends \
    curl \
    git \
    zsh \
    ca-certificates \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

# Install LATEST Neovim (Required for LazyVim)
RUN curl -LO https://github.com/neovim/neovim/releases/latest/download/nvim-linux-x86_64.tar.gz \
    && tar -C /opt -xzf nvim-linux-x86_64.tar.gz
ENV PATH="/opt/nvim-linux-x86_64/bin:${PATH}"

# Set Zsh as default shell
ENV SHELL=/bin/zsh

# Node dependency logic
COPY package*.json ./
RUN npm install
COPY . .

CMD ["npx", "expo", "start", "--web"]
