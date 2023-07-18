

add_library(gmod_headers INTERFACE)
target_include_directories(gmod_headers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/gmod_headers/include)

# Originally from facepunch cmake build system, modified to be slightly less painful
function(set_gmod_suffix_prefix library)
	set_target_properties(${library} PROPERTIES PREFIX "gmsv_")
	if(APPLE)
		if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
			set_target_properties(${library} PROPERTIES SUFFIX "_osx.dll")
		else()
			set_target_properties(${library} PROPERTIES SUFFIX "_osx64.dll")
		endif()
	elseif(UNIX)
		if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
			set_target_properties(${library} PROPERTIES SUFFIX "_linux.dll")
		else()
			set_target_properties(${library} PROPERTIES SUFFIX "_linux64.dll")
		endif()
	elseif(WIN32)
		if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
			set_target_properties(${library} PROPERTIES SUFFIX "_win32.dll")
		else()
			set_target_properties(${library} PROPERTIES SUFFIX "_win64.dll")
		endif()
	endif()
endfunction()


add_library(lcpu_native SHARED
	src/main.cpp
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	target_link_options(lcpu_native PRIVATE "-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/gmod_abi.ver")
endif()

target_link_libraries(lcpu_native
	gmod_headers
	lucore::lucore
	riscv::riscv
)

set_gmod_suffix_prefix(lcpu_native)

