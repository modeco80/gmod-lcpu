cmake_minimum_required(VERSION 3.15)
project(lcpu-native
	DESCRIPTION "Superproject for LCPU GMOD Native Module"
)

add_subdirectory(lucore)

# RISC-V emulation library
add_subdirectory(riscv)

# Server-side emulation/assembler library
#add_subdirectory(gmsv_lcpu)
