#!/bin/bash

set -euo pipefail

SYSROOT="${RPI_SYSROOT:?RPI_SYSROOT is not set}"
HOST="${RPI_HOST:?RPI_HOST is not set}"

mkdir -p "$SYSROOT"

rsync --progress -avHAX --numeric-ids --delete \
    --exclude='/dev/*' \
    --exclude='/proc/*' \
    --exclude='/sys/*' \
    --exclude='/tmp/*' \
    --exclude='/run/*' \
    --exclude='/mnt/*' \
    --exclude='/var/*' \
    --exclude='/etc/*' \
    --exclude='/root/' \
    --exclude='/usr/lib/netplan/*' \
    --exclude='/media/*' \
    --exclude='/lost+found' \
    --exclude='/swapfile' \
    "$HOST":/ \
    "$SYSROOT/"


