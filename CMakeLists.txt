cmake_minimum_required(VERSION 3.0)

project(webserver)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/extern/build/install/include)
set(LIBRARY_DIR ${CMAKE_SOURCE_DIR}/extern/build/install/lib)

find_library(LIBEVENT_LIB NAME libevent.a PATHS ${LIBRARY_DIR})

link_directories(${LIBRARY_DIR})

add_executable(server server.cpp)

target_include_directories(server PRIVATE ${INCLUDE_DIR})

target_link_libraries(server ${LIBEVENT_LIB})