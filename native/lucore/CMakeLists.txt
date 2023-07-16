cmake_minimum_required(VERSION 3.15)

project(lucore 
    DESCRIPTION "lucore - shared core stuff for the lcpu native projects"
    LANGUAGES CXX
)

add_library(lucore INTERFACE)

target_compile_features(lucore INTERFACE cxx_std_20)
target_include_directories(lucore INTERFACE ${PROJECT_SOURCE_DIR}/include)

add_library(lucore::lucore ALIAS lucore)
