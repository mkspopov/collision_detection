cmake_minimum_required(VERSION 3.17)

project(collision_detection)

set(CMAKE_CXX_STANDARD 20)

set(
    SOURCES
    blocking_object.cpp
    camera.cpp
    collider.cpp
    direction.cpp
    object.cpp
    shape.cpp
    square.cpp
    utils.cpp
)

add_executable(
    collision_detection
    main.cpp
    ${SOURCES}
)

include(FindPkgConfig)

pkg_search_module(SDL2 REQUIRED sdl2)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(collision_detection ${SDL2_LIBRARIES})
