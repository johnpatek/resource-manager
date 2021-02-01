cmake_minimum_required(VERSION 3.0)

project(webserver-extern)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)
set(EVENT__DISABLE_MBEDTLS ON CACHE BOOL "Disable mbedTLS")
add_subdirectory(libevent)