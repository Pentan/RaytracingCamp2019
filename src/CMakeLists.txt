# src/CMakeLists.txt
cmake_minimum_required (VERSION 3.3)
project (PinkyPi)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED True)

set (PINKYPI_SOURCE_DIR ${CMAKE_SOURCE_DIR}/sources)
set (PINKYPI_TTEST_DIR ${CMAKE_SOURCE_DIR}/tests)
set (PINKYPI_THIRDPARTY_DIR ${CMAKE_SOURCE_DIR}/thirdparty)

add_subdirectory(sources)
add_subdirectory(tests)
add_subdirectory(thirdparty)
