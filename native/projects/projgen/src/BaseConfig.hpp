#pragma once 

/// These are the hardcoded defaults
#define PROJGEN_CC "riscv32-unknown-elf-gcc"
#define PROJGEN_CXX "riscv32-unknown-elf-g++"
#define PROJGEN_BASE_C_FLAGS "-ffreestanding -fno-stack-protector -fdata-sections -ffunction-sections -march=rv32ima -mabi=ilp32"
#define PROJGEN_BASE_CC_FLAGS "-ffreestanding -fno-stack-protector -fdata-sections -ffunction-sections -march=rv32ima -mabi=ilp32"
#define PROJGEN_BASE_LD_FLAGS "-nostdlib"
