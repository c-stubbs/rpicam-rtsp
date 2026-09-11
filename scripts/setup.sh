#!/bin/bash

set -e

case "$1" in
    build)
        sudo apt install -y \
            libglib2.0-dev \
            libgstreamer1.0-dev \
            libgstrtspserver-1.0-dev \
            libspdlog-dev \
            libtomlplusplus-dev
        ;;

    runtime)
        sudo apt install -y \
            libgstreamer1.0-0 \
            libgstrtspserver-1.0-0 \
            libfmt10 \
            rpicam-apps
        ;;

    *)
        echo "Usage: $0 {build|runtime}"
        exit 1
        ;;
esac
