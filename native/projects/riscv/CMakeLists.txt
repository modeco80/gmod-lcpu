cmake_minimum_required(VERSION 3.15)

project(riscv_emu 
    DESCRIPTION "rv32-ima emulation library"
    LANGUAGES CXX
)

add_library(riscv
	src/Bus.cpp
	src/Devices/RamDevice.cpp
)

target_compile_features(riscv PUBLIC cxx_std_20)
target_include_directories(riscv PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(riscv PUBLIC
	# lucore is a public dependency, since it is (techinically) used in public api surfaces
	lucore::lucore
)

add_library(riscv::riscv ALIAS riscv)
