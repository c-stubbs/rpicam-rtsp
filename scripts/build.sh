#!/bin/bash

set -e

cmake -S . -B build \
    -G Ninja \
    --toolchain ${PWD}/cmake/toolchains/native.cmake \
    -DCMAKE_INSTALL_PREFIX=${PWD}/install

cmake --build build --parallel 1
    
cmake --install build

