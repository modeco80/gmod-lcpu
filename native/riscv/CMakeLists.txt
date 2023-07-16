cmake_minimum_required(VERSION 3.15)

project(riscv_emu 
    DESCRIPTION "rv32-ima emulation library"
    LANGUAGES CXX
)

add_library(riscv
	src/Bus.cpp
)

target_compile_features(riscv PUBLIC cxx_std_20)
target_include_directories(riscv PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(riscv::riscv ALIAS riscv)
