cmake_minimum_required(VERSION 3.2)
project(bubblestest)

find_package(PkgConfig)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g")
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

set(CFLAGS ${DEPS_CFLAGS} ${DEPS_CFLAGS_OTHER})
set(LIB_PATHS ${DEPS_LIBRARY_DIRS})

add_definitions(${CFLAGS})
link_directories(${LIB_PATHS})

# Sources
set(SOURCES main.cpp qube_actor.cpp)

add_executable(bubblestest ${SOURCES})
target_link_libraries(${PROJECT_NAME} bubbles)

