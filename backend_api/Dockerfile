FROM ubuntu:25.10 AS crow-base

RUN apt-get update && apt-get install -y --no-install-recommends \
  ca-certificates \
  git \
  build-essential \
  cmake \
  ninja-build \
  pkg-config \
  libasio-dev \
  zlib1g-dev \
  libssl-dev \
  && rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/CrowCpp/Crow.git /tmp/Crow \
  && cmake -S /tmp/Crow -B /tmp/Crow/build \
  -DCROW_BUILD_EXAMPLES=OFF \
  -DCROW_BUILD_TESTS=OFF \
  -DCMAKE_BUILD_TYPE=Release \
  && cmake --build /tmp/Crow/build \
  && cmake --install /tmp/Crow/build \
  && rm -rf /tmp/Crow

FROM crow-base AS dev
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
  npm \
  && rm -rf /var/lib/apt/lists/*

RUN apt-get update && apt-get install -y --no-install-recommends \
  curl \
  xz-utils \
  python3 \
  python3-venv \
  python3-pip \
  unzip \
  build-essential \
  ripgrep \
  clang-format \
  && rm -rf /var/lib/apt/lists/*

RUN curl -LO https://github.com/neovim/neovim/releases/latest/download/nvim-linux-x86_64.tar.gz \
  && rm -rf /opt/nvim-linux-x86_64 \
  && tar -C /opt -xzf nvim-linux-x86_64.tar.gz

RUN git clone https://github.com/LazyVim/starter ~/.config/nvim

ENV PATH="/opt/nvim-linux-x86_64/bin:${PATH}"

WORKDIR /workspace/
