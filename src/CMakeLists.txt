cmake_minimum_required(VERSION 3.16.3)

project(collision_detection)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib)
include_directories(lib)

add_executable(
    collision_detection
    main.cpp
)

include(FindPkgConfig)

pkg_search_module(SDL2 REQUIRED sdl2)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(collision_detection ${SDL2_LIBRARIES})

target_link_libraries(collision_detection collision_detection_lib)
