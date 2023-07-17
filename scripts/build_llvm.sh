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
	-DLLVM_TARGETS_TO_BUILD="RISCV" \
	-DLLVM_DEFAULT_TARGET_TRIPLE="riscv32-none-gnu"

ninja -C /build/llvm-build/build -j $JOBS
ninja -C /build/llvm-build install

# Build compiler-rt with our toolchain.
cmake -Wno-dev -GNinja -S /build/llvm/compiler-rt -B /build/llvm-build/build-compiler-rt \
	-DCMAKE_C_COMPILER="/build/llvm-build/install/bin/clang" \
	-DCMAKE_CXX_COMPILER="/build/llvm-build/install/bin/clang++" \
	-DCMAKE_C_FLAGS="-target riscv32-none-gnu" \
	-DCMAKE_CXX_FLAGS="-target riscv32-none-gnu" \
	-DCMAKE_INSTALL_PREFIX="/build/llvm-build/install/lib/clang/${LLVM_VERSION}" \
	-DLLVM_CONFIG_PATH="/build/llvm-build/install/bin/llvm-config" \
	-DCOMPILER_RT_BAREMETAL_BUILD=ON \
	-DCOMPILER_RT_BUILD_SANITIZERS=OFF \
	-DCOMPILER_RT_BUILD_LIBFUZZER=OFF \
	-DCOMPILER_RT_BUILD_PROFILE=OFF \
	-DCOMPILER_RT_BUILD_MEMPROF=OFF \
	-DCOMPILER_RT_BUILD_ORC=OFF

ninja -C /build/llvm-build/build-compiler-rt -j $JOBS
ninja -C /build/llvm-build/build-compiler-rt install
