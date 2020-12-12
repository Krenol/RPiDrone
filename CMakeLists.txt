# CMakeList.txt : CMake project for RPiComponents, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.4)
set(CMAKE_CXX_STANDARD 14)  # enable C++14 standard
set(CMAKE_CXX_STANDARD_REQUIRED True)

project (RPiDrone VERSION 1.0 LANGUAGES CXX)


set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# Let's ensure -std=c++xx instead of -std=g++xx
set(CMAKE_CXX_EXTENSIONS OFF)

# Let's nicely support folders in IDEs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(rpidrone)

if (NOT "$ENV{BUILD_TEST}" STREQUAL "")
    set(BUILD_TEST "$ENV{BUILD_TEST}" CACHE INTERNAL "Copied from environment variable")
endif()

#configure_file(
#    ${CMAKE_SOURCE_DIR}/globals.hpp.in
#    ${CMAKE_SOURCE_DIR}/rpidrone/src/globals.hpp
#)