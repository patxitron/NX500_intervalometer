#!/bin/sh
THIS_DIR="$(dirname "$(readlink -f "${0}")")"
cd "${THIS_DIR}/beast"
if ! [ -f CMakeLists.txt ]; then
    git submodule init
    git submodule update
fi

