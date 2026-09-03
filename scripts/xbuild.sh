#!/bin/bash

set -e

cmake -S . -B xbuild \
    --toolchain ${PWD}/cmake/toolchains/raspberry_pi.cmake \
    -DCMAKE_INSTALL_PREFIX=${PWD}/xinstall

cmake --build xbuild --parallel 1 

cmake --install xbuild

