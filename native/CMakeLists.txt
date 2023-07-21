cmake_minimum_required(VERSION 3.15)
project(lcpu-native
	DESCRIPTION "Superproject for LCPU GMOD Native Module"
)

# Lucore (lily utilities core)
add_subdirectory(projects/lucore)

# RISC-V emulator library
add_subdirectory(projects/riscv)

add_subdirectory(projects/riscv_test_harness)

# Garry's Mod native bindings to RISC-V emulator
# Also lua device stuff
add_subdirectory(projects/lcpu)
