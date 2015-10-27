cmake_minimum_required(VERSION 3.3)
project(Pongl)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${OpenGL_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIR})

set(SOURCE_FILES main.cpp)
add_executable(Pongl ${SOURCE_FILES})


target_link_libraries(Pongl ${GLEW_LIBRARY})
target_link_libraries(Pongl ${SDL2_LIBRARY})
target_link_libraries(Pongl ${OPENGL_LIBRARY})