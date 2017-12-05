#!/bin/sh
THIS_DIR="$(dirname "$(readlink -f "${0}")")"
if [ -d "${THIS_DIR}/${1}" ]; then
    cd "${THIS_DIR}/${1}"
    case "${1}" in
    beast | json)
        if ! [ -f CMakeLists.txt ]; then
            git submodule init
            git submodule update
        fi
        ;;
    xdotool)
        if ! [ -f Makefile ]; then
            git submodule init
            git submodule update
        fi
        CC=${THIS_DIR}/buildroot/output/host/usr/bin/arm-buildroot-linux-musleabi-gcc
        CXX=${THIS_DIR}/buildroot/output/host/usr/bin/arm-buildroot-linux-musleabi-g++
        AR=${THIS_DIR}/buildroot/output/host/usr/bin/arm-buildroot-linux-musleabi-ar
        export CC CXX AR
        make libxdo.a
        ;;
    buildroot)
        if ! [ -f Makefile ]; then
            git submodule init
            git submodule update
        fi
        cp -f ../buildroot-nx500.config .config
        make oldconfig
        make
        ;;
    *)
        echo "Unknown submodule"
        ;;
    esac
fi
