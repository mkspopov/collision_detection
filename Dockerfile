FROM clangbuiltlinux/ubuntu:latest as build

RUN apt-get update && \
    apt-get install -y \
        libsdl2-dev \
        cmake

ADD ./src /app/src

WORKDIR /app/build

ARG LLVM_VERSION=12
ENV CXX=clang++-${LLVM_VERSION}

RUN cmake ../src -DCMAKE_BUILD_TYPE=Release && \
    cmake --build .

RUN groupadd -r sample && useradd -r -g sample sample
USER sample

CMD ["./collision_detection"]
