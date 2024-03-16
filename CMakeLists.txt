cmake_minimum_required(VERSION 3.10)

project(TCPServer)

include_directories(include)

file(GLOB SRCS src/*.cpp)

add_executable(server ${SRCS})