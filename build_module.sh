#!/bin/bash

# Build the LCPU addon for the reccomended environment
# and install it into the proper directory gmod wants native modules to be.
# This expects to be ran in [gmod]/addons/[addon folder].

set -x

cmake -Wno-dev -GNinja -S native -B build -DCMAKE_BUILD_TYPE=Release
ninja -C build

[[ ! -d '../../lua/bin' ]] && {
	mkdir -p ../../lua/bin
}

cp -v build/projects/lcpu/*.dll ../../lua/bin
