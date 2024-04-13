# 
cmake_minimum_required(VERSION 3.0)


project(test)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") 
include_directories(${PROJECT_SOURCE_DIR}/inc)

add_definitions(-DDEBUG --std=c++17 -fPIC -Wall -Werror)
# add_definitions(-DDEBUG --std=c++17 -fPIC -Wall)

set(CMAKE_BUILD_TYPE "Debug")
# set(CMAKE_BUILD_TYPE "Release")
# set(CMAKE_BUILD_TYPE "MinSizeRel")

add_executable(test test.cpp)

target_link_libraries(test)
