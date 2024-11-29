cmake_minimum_required(VERSION 3.10)

project(TCPServer)

# C++ standard version
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Paths to search for libraries and header files
#include_directories(/usr/local/include)
#include_directories(${PROJECT_SOURCE_DIR}/libs)
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB SRCS src/*.cpp)

add_definitions(-DPROJECT_SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\")

add_executable(server ${SRCS})