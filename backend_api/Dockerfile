ARG UBUNTU_VERSION=24.04

# -----------------------------
# 1) Build stage (compile)
# -----------------------------
FROM ubuntu:${UBUNTU_VERSION} AS build
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
  ca-certificates \
  git \
  curl \
  build-essential \
  cmake \
  ninja-build \
  pkg-config \
  libasio-dev \
  libssl-dev \
  zlib1g-dev \
  libpqxx-dev \
  libpq-dev \
  postgresql-client \
  && rm -rf /var/lib/apt/lists/*

# Install Crow (needed because you use find_package(Crow CONFIG REQUIRED))
RUN git clone --depth 1 https://github.com/CrowCpp/Crow.git /tmp/Crow \
  && cmake -S /tmp/Crow -B /tmp/Crow/build -G Ninja \
  -DCROW_BUILD_EXAMPLES=OFF \
  -DCROW_BUILD_TESTS=OFF \
  -DCMAKE_BUILD_TYPE=Release \
  && cmake --build /tmp/Crow/build \
  && cmake --install /tmp/Crow/build \
  && rm -rf /tmp/Crow

WORKDIR /src
COPY . .

# Build everything (including tests)
RUN cmake -S . -B build -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DWEBSERVER_BUILD_TESTS=ON \
  && cmake --build build

# -----------------------------
# 2) Test stage (run tests)
# -----------------------------
FROM ubuntu:${UBUNTU_VERSION} AS test
ENV DEBIAN_FRONTEND=noninteractive

# Runtime deps for your binaries + ctest runner
RUN apt-get update && apt-get install -y --no-install-recommends \
  ca-certificates \
  cmake \
  libpqxx-dev \
  libpq-dev \
  libssl-dev \
  zlib1g-dev \
  && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY --from=build /src/build /src/build

CMD ["bash", "-lc", "set -euo pipefail; cd /src/build; ctest"]

