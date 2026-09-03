#!/bin/bash

set -e

cmake -S . -B build \
    -G Ninja \
    -DCMAKE_INSTALL_PREFIX=${PWD}/install

cmake --build build --parallel 1 -- -d stats
    
cmake --install build

