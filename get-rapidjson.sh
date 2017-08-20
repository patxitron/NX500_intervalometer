#!/bin/sh
THIS_DIR="$(dirname "$(readlink -f "${0}")")"
cd "${THIS_DIR}/rapidjson"
if ! [ -d include ]; then
    git submodule init
    git submodule update
fi
