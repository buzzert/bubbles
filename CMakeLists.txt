cmake_minimum_required(VERSION 3.2)
project(bubbles)

find_package(PkgConfig)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g")
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Project specific dependencies
set(PROJECT_DEPS sdl2 SDL2_image freetype2 pango pangoft2 pangocairo cairo)
pkg_check_modules(DEPS REQUIRED ${PROJECT_DEPS})

set(CFLAGS ${DEPS_CFLAGS} ${DEPS_CFLAGS_OTHER})
set(LIB_PATHS ${DEPS_LIBRARY_DIRS})
add_definitions(${CFLAGS})
link_directories(${LIB_PATHS})

add_subdirectory(example)

# Sources
file(GLOB_RECURSE SOURCES "src/bubbles/*.cpp")
file(GLOB_RECURSE HEADERS "src/bubbles/*.h")

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME} ${DEPS_LIBRARIES})

