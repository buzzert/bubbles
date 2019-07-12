cmake_minimum_required(VERSION 3.0.2)

project (bubbles)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

find_package(PkgConfig REQUIRED)
find_package(X11 REQUIRED)
pkg_check_modules(CAIRO REQUIRED cairo)
pkg_check_modules(PANGO REQUIRED pangocairo)
pkg_check_modules(XFIXES REQUIRED xfixes)

add_library(bubbles STATIC
  src/bubbles/bubbles.h
  src/bubbles/core/foundation.h
  src/bubbles/core/color.h
  src/bubbles/core/defines.h
  src/bubbles/core/geometry.h
  src/bubbles/core/geometry.cpp
  src/bubbles/core/x11_support.h
  src/bubbles/core/x11_support.c

  src/bubbles/actor/actor.h
  src/bubbles/actor/actor.cpp
  src/bubbles/actor/actor_grid.h
  src/bubbles/actor/actor_grid.cpp
  src/bubbles/actor/label_actor.h
  src/bubbles/actor/label_actor.cpp

  src/bubbles/scene/scene.h
  src/bubbles/scene/scene.cpp
)

target_include_directories(bubbles PUBLIC src/) 

target_include_directories(bubbles PUBLIC
    ${CAIRO_INCLUDE_DIRS}
    ${PANGO_INCLUDE_DIRS}
)
target_link_libraries(bubbles
    ${CAIRO_LIBRARIES}
    ${PANGO_LIBRARIES}
    ${X11_LIBRARIES}
    ${XFIXES_LIBRARIES}
)

