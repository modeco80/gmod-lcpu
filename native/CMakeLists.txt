cmake_minimum_required(VERSION 3.15)
project(lcpu-native
	DESCRIPTION "Superproject for LCPU GMOD Native Module"
)

add_subdirectory(projects/lucore)
add_subdirectory(projects/lucore_test)

# RISC-V emulation library
add_subdirectory(projects/riscv)

# Garry's Mod bindings
add_subdirectory(projects/lcpu)
