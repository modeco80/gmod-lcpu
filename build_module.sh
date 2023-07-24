#!/bin/bash

# Build the LCPU addon for the reccomended environment
# and install it into the proper directory gmod wants native modules to be.

set -x

# where your game server is
GS_PATH="/home/lily/gs/gmod"

cmake -Wno-dev -GNinja -S native -B module_build -DCMAKE_BUILD_TYPE=Release
ninja -C module_build

[[ ! -d "$GS_PATH/garrysmod/lua/bin" ]] && {
	mkdir -p $GS_PATH/garrysmod/lua/bin
}

cp -v module_build/projects/lcpu/*.dll $GS_PATH/garrysmod/lua/bin
