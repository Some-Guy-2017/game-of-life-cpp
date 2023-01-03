cmake_minimum_required(VERSION 3.10)

project(game_of_life C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

FILE(GLOB SRCS src/*.cpp src/*.hpp)
add_executable(game-of-life ${SRCS})

find_package(SDL2 REQUIRED)
target_link_libraries(game-of-life ${SDL2_LIBRARIES})
include_directories(${SDL2_INCLUDE_DIRS})
