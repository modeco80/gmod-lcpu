//! These are the hardcoded defaults projgen uses for configuring Makefiles.
//! Only change these if you know what they do.
#pragma once 

#define PROJGEN_CC "riscv32-unknown-elf-gcc"
#define PROJGEN_CXX "riscv32-unknown-elf-g++"
#define PROJGEN_LD "riscv32-unknown-elf-gcc"
#define PROJGEN_OBJCOPY "riscv32-unknown-elf-objcopy"
#define PROJGEN_BASE_C_FLAGS "-ffreestanding -fno-stack-protector -fdata-sections -ffunction-sections -march=rv32ima -mabi=ilp32"
#define PROJGEN_BASE_CC_FLAGS "-ffreestanding -fno-rtti -fno-exceptions -fno-stack-protector -fdata-sections -ffunction-sections -march=rv32ima -mabi=ilp32"
#define PROJGEN_BASE_LD_FLAGS "-nostdlib"
