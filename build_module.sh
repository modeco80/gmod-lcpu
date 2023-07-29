#!/bin/bash

# Build the LCPU native module for both linux32 and linux64
# and install it into the proper directory gmod wants native modules to be.

set -x

# where your game server is
GS_PATH="/home/lily/gs/gmod"

# make the module build directory
[[ ! -d "module_build" ]] && { 
	mkdir module_build
}

# $1 cmake build dir
# $@ any other args to cmake
cmake_gen() {
	local BD=$1
	shift
	cmake -Wno-dev -GNinja -S native -B module_build/$BD $@ -DCMAKE_BUILD_TYPE=Release
}

# $1 cmake build dir
cmake_build() {
	ninja -C module_build/$1
}

build_and_place() {
	cmake_gen linux32 --toolchain $PWD/native/cmake/linux32-toolchain.cmake
	cmake_gen linux64
	cmake_build linux32
	cmake_build linux64

	# GMod doesn't actually make the lua/bin directory on its own
	# so we have to check if it exists first and make if it doesn't
	[[ ! -d "$GS_PATH/garrysmod/lua/bin" ]] && {
		mkdir -p $GS_PATH/garrysmod/lua/bin
	}

	cp -v module_build/linux32/projects/lcpu/*.dll $GS_PATH/garrysmod/lua/bin
	cp -v module_build/linux64/projects/lcpu/*.dll $GS_PATH/garrysmod/lua/bin
}

build_and_place
