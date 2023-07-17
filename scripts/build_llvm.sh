#!/bin/bash

# This script builds the llvm components used by the LCPU addon.
# This is run in a debian container to try and keep everything ABI stable.

JOBS=$(nproc)

# Configure LLVM. THis is a mouthful, but it's only a mouthful because we specialize things we don't
# want in our minimal build of the LLVM project/clang.
cmake -Wno-dev -GNinja -S /build/llvm/llvm -B /build/llvm-build/build \
	-DCMAKE_BUILD_TYPE=MinSizeRel \
	-DCMAKE_INSTALL_PREFIX="/build/llvm-build/install" \
	-DLLVM_INCLUDE_TESTS=OFF \
	-DLLVM_ENABLE_PROJECTS="clang;lld" \
	-DLLVM_TARGETS_TO_BUILD="RISCV" \
	-DLLVM_DEFAULT_TARGET_TRIPLE="riscv32-none-gnu"

ninja -C /build/llvm-bin/build -j $JOBS

# TODO: Build compiler-rt with our toolchain.
# This currently does not work as a single build (why would it), we instead
# need to stage things (the first building clang + lld), and this being the last stage currently
#cmake -Wno-dev -GNinja -S llvm_src/compiler-rt \
#	-DCOMPILER_RT_BUILD_SANITIZERS=OFF \
#	-DCOMPILER_RT_BUILD_LIBFUZZER=OFF \
#	-DCOMPILER_RT_BUILD_PROFILE=OFF \
#	-DCOMPILER_RT_BUILD_MEMPROF=OFF \
#	-DCOMPILER_RT_BUILD_ORC=OFF \
#	-DCOMPILER_RT_BAREMETAL_BUILD=ON
