cmake_minimum_required(VERSION 3.3)
project(Pongl)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

set(SOURCE_FILES main.cpp)
add_executable(Pongl ${SOURCE_FILES})
target_link_libraries(Pongl ${SDL2_LIBRARY})