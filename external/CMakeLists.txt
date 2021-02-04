cmake_minimum_required(VERSION 3.0)

project(webserver-extern)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)
set(EVENT__DISABLE_MBEDTLS ON CACHE BOOL "Disable mbedTLS")
set(RAPIDJSON_BUILD_DOC OFF CACHE BOOL "No rapidjson docs")
set(RAPIDJSON_BUILD_EXAMPLES OFF CACHE BOOL "No rapidjson examples")
set(RAPIDJSON_BUILD_TESTS  OFF CACHE BOOL "No rapidjson tests")
set(RAPIDJSON_BUILD_THIRDPARTY_GTEST OFF CACHE BOOL "No rapidjson gtest")
add_subdirectory(ubiq-c-cpp)
add_subdirectory(libevent)
add_subdirectory(rapidjson)
