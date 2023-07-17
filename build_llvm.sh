#!/bin/bash

# this script builds llvm components

LLVM_VERSION=16.0.0
LLVM_URL="https://github.com/llvm/llvm-project/releases/download/llvmorg-$LLVM_VERSION/llvm-project-$LLVM_VERSION.src.tar.xz"
LLVM_FILENAME="llvm-project-$LLVM_VERSION.src.tar.xz"


# download/extract LLVM (if need be)
[[ ! -d 'llvm_src' ]] && {
	curl -LO $LLVM_URL
	tar xf $LLVM_FILENAME
	mv llvm-project-$LLVM_VERSION.src llvm_src/
	rm $LLVM_FILENAME
};

# Configure LLVM. THis is a mouthful, but it's only a mouthful because we specialize things we don't
# want in our minimal build of the LLVM project/clang.
# -DLLVM_ENABLE_LTO=Thin 
cmake -GNinja -S llvm_src/llvm -B llvm_build \
	-DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_C_COMPILER=clang \
	-DCMAKE_CXX_COMPILER=clang++ \
	-DCMAKE_INSTALL_PREFIX="$PWD/lua/bin/llvm" \
	-DLLVM_USE_LINKER=lld \
	-DLLVM_INCLUDE_TESTS=OFF \
	-DLLVM_ENABLE_PROJECTS="clang;lld" \
	-DLLVM_TARGETS_TO_BUILD="RISCV" \
	-DLLVM_DEFAULT_TARGET_TRIPLE="riscv32-none-gnu"

# TODO: Configure and build compiler-rt with our toolchain.
#cmake -GNinja -S llvm_src/compiler-rt \
#	-DCOMPILER_RT_BUILD_SANITIZERS=OFF \
#	-DCOMPILER_RT_BUILD_LIBFUZZER=OFF \
#	-DCOMPILER_RT_BUILD_PROFILE=OFF \
#	-DCOMPILER_RT_BUILD_MEMPROF=OFF \
#	-DCOMPILER_RT_BUILD_ORC=OFF \
#	-DCOMPILER_RT_BAREMETAL_BUILD=ON

# Build it
cmake --build llvm_build -j $(($(nproc) + 1))
