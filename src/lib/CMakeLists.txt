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

add_library(
    collision_detection_lib
    STATIC
    ${SOURCES}
)

include(FindPkgConfig)

pkg_search_module(SDL2 REQUIRED sdl2)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(collision_detection_lib ${SDL2_LIBRARIES})
