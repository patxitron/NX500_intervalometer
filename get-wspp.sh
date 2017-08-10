#!/bin/sh
THIS_DIR="$(dirname "$(readlink -f "${0}")")"
cd "${THIS_DIR}/websocketpp"
if ! [ -d websocketpp ]; then
    git submodule init
    git submodule update
fi
