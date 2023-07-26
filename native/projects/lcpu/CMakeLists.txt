include(./gmod_headers.cmake)

add_library(lcpu_native SHARED
	src/main.cpp
	src/LcpuGlobals.cpp
	src/LuaCpu.cpp
	src/LuaDevice.cpp

	# Support code
	src/SourceSink.cpp
	src/LuaShared.cpp
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	target_link_options(lcpu_native PRIVATE 
		"-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/gmod_abi.ver"
		# screw it, dude
		"-static-libstdc++"
		"-static-libgcc"
		)
endif()

target_link_libraries(lcpu_native
	gmod_headers
	lucore::lucore
	riscv::riscv
)

set_gmod_suffix_prefix(lcpu_native)

