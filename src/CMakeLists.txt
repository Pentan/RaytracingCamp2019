# src/CMakeLists.txt
# macOS example: cmake -DCMAKE_OSX_ARCHITECTURES="Intel" -G Xcode ../src

cmake_minimum_required (VERSION 3.8)
project (PinkyPi)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

set (PINKYPI_SOURCE_DIR ${CMAKE_SOURCE_DIR}/sources)
set (PINKYPI_TTEST_DIR ${CMAKE_SOURCE_DIR}/tests)
set (PINKYPI_THIRDPARTY_DIR ${CMAKE_SOURCE_DIR}/thirdparty)

add_subdirectory(sources)
add_subdirectory(tests)
add_subdirectory(thirdparty)
