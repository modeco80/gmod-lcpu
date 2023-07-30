cmake_minimum_required(VERSION 3.15)
project(lcpu-native
	DESCRIPTION "Superproject for LCPU GMOD Native Module"
)

option(LCPU_BUILD_UTILITIES "Build LCPU utiltiies" ON)
option(LCPU_BUILD_MODULE "Build GMod module" ON)

# Lucore (lily utilities core)
add_subdirectory(projects/lucore)

if(LCPU_BUILD_MODULE)
	add_subdirectory(projects/riscv)
	add_subdirectory(projects/lcpu)
endif()

if(LCPU_BUILD_UTILITIES)
	add_subdirectory(projects/projgen)
endif()
