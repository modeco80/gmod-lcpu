cmake_minimum_required(VERSION 3.15)
project(lcpu-projgen
	DESCRIPTION "LCPU Project Generator - generates a Makefile from project.json"
)

set(DAW_USE_CPP17_NAMES OFF)
add_subdirectory(third_party/daw_json_link)

add_executable(lcpu_projgen
	# driver
	src/main.cpp
)

target_link_libraries(lcpu_projgen
	lucore::lucore
	daw::json_link
)
