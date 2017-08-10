#!/bin/sh
THIS_DIR="$(dirname "$(readlink -f "${0}")")"
cd "${THIS_DIR}/buildroot"
if ! [ -f Makefile ]; then
    git submodule init
    git submodule update
fi
cp -f ../buildroot-nx500.config .config
make oldconfig
make

