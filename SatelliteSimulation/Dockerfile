FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    pkg-config \
    protobuf-compiler \
    libprotobuf-dev \
    libgrpc-dev \
    libssl-dev \
    libz-dev \
    libc-ares-dev \
    libre2-dev \
    libgtest-dev \
    && rm -rf /var/lib/apt/lists/*

RUN cd /usr/src/gtest && cmake . && make && mv lib/*.a /usr/lib/

WORKDIR /app

COPY . .

RUN apt-get update && apt-get install -y libgrpc++-dev libprotobuf-dev protobuf-compiler-grpc


RUN mkdir -p build && cd build && \
    cmake .. && make -j$(nproc)


CMD ["./build/SatelliteSimulation"]