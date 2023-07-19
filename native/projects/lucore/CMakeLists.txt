cmake_minimum_required(VERSION 3.15)

project(lucore 
    DESCRIPTION "lucore - shared core stuff for the lcpu native projects"
    LANGUAGES CXX
)

add_library(lucore
	# Assertion/Check failure handling
	src/Assert.cpp

	# Logging functionality
	src/Logger.cpp
	src/StdoutSink.cpp

	# Dynamic library wrappers
	src/Library.cpp
)

# Target-specific Lucore sources.
if(WIN32)
	target_sources(lucore PRIVATE
		src/OsLibrary.win32.cpp
	)
elseif(UNIX)
	target_sources(lucore PRIVATE
		src/OsLibrary.linux.cpp
	)
endif()

target_compile_features(lucore PUBLIC cxx_std_20)
target_include_directories(lucore PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(lucore::lucore ALIAS lucore)
