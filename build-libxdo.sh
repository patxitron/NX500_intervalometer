#!/bin/sh
THIS_DIR="$(dirname "$(readlink -f "${0}")")"
cd "${THIS_DIR}/xdotool"
if ! [ -f Makefile ]; then
    git submodule init
    git submodule update
fi
CC=${THIS_DIR}/buildroot/output/host/usr/bin/arm-buildroot-linux-musleabi-gcc
CXX=${THIS_DIR}/buildroot/output/host/usr/bin/arm-buildroot-linux-musleabi-g++
AR=${THIS_DIR}/buildroot/output/host/usr/bin/arm-buildroot-linux-musleabi-ar
export CC CXX AR
make libxdo.a
