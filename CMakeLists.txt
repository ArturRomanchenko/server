cmake_minimum_required(VERSION 3.10)

project(TCPServer)

# C++ standard version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find the nlohmann_json library
#find_package(nlohmann_json 3.2.0 REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCES
    src/main.cpp
    src/network/server.cpp
    src/utils/logs.cpp
)

#add_definitions(-DPROJECT_SOURCE_DIR=\"${CMAKE_SOURCE_DIR}\")

add_executable(server ${SOURCES})

target_link_libraries(server pthread)
#target_link_libraries(server PRIVATE nlohmann_json::nlohmann_json)