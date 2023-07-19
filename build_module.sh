#!/bin/bash


# Build the LCPU addon for the reccomended environment

set -x


cmake -Wno-dev -GNinja -S native -B build -DCMAKE_BUILD_TYPE=Release
ninja -C build

[[ ! -d '../../lua/bin' ]] && {
	mkdir -p ../../lua/bin
}

cp -v build/projects/lcpu/*.dll ../../lua/bin
