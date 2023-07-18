cmake_minimum_required(VERSION 3.15)

project(lucore 
    DESCRIPTION "lucore - shared core stuff for the lcpu native projects"
    LANGUAGES CXX
)

add_library(lucore
	src/Assert.cpp

	src/Logger.cpp
	src/StdoutSink.cpp

	src/OsLibrary.cpp
	src/Library.cpp
)

target_compile_features(lucore PUBLIC cxx_std_20)
target_include_directories(lucore PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(lucore::lucore ALIAS lucore)
