#!/bin/bash

# This script builds the llvm components used by the LCPU addon.
# This is run in a debian container to try and keep everything ABI stable.

# Act like make, sort of, and fail the whole script if some part fails
set -e

JOBS=$(nproc)

# Configure LLVM. THis is a mouthful, but it's only a mouthful because we specialize things we don't
# want in our minimal build of the LLVM project/clang.
cmake -Wno-dev -GNinja -S /build/llvm/llvm -B /build/llvm-build/build \
	-DCMAKE_BUILD_TYPE=MinSizeRel \
	-DCMAKE_INSTALL_PREFIX="/build/llvm-build/install" \
	-DLLVM_INCLUDE_TESTS=OFF \
	-DLLVM_ENABLE_PROJECTS="clang;lld" \
	-DLLVM_ENABLE_RUNTIMES="compiler-rt" \
	-DCOMPILER_RT_BAREMETAL_BUILD=ON \
	-DLLVM_TARGETS_TO_BUILD="RISCV" \
	-DLLVM_DEFAULT_TARGET_TRIPLE="riscv32-unknown-gnu"

ninja -C /build/llvm-build/build -j $JOBS
ninja -C /build/llvm-build/build install
