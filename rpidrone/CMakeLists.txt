# CMakeList.txt : CMake project for RPiComponents, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.4)
set(CMAKE_CXX_STANDARD 11)  # enable C++11 standard

project (RPiDrone)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
 
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
 
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(RPISOCKET_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/RPiSocket CACHE INTERNAL "Path to include folder for RPiSocket")
set(RPICOMP_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/RPiComponents CACHE INTERNAL "Path to include folder for RPiComponents")
set(CONTROLLERS_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/CppControllers CACHE INTERNAL "Path to include folder for CppControllers")

set(RPICDRONE_PROJECT_SRCS 
${PROJECT_SOURCE_DIR}/test.cpp
${PROJECT_SOURCE_DIR}/queue.cpp
)

include_directories(${PROJECT_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${RPISOCKET_INCLUDE_DIR})
include_directories(${RPICOMP_INCLUDE_DIR})
include_directories(${CONTROLLERS_INCLUDE_DIR})

#add_subdirectory (external/RPiSocket/rpisocket)
#add_subdirectory (external/RPiComponents/rpicomponents)
#add_subdirectory (external/CppControllers/controllers)

add_executable (RPiDrone ${RPICDRONE_PROJECT_SRCS})

# Own libs
#find_library(RPiComponents REQUIRED)
#find_library(RPiSocket REQUIRED)
#find_library(Controllers REQUIRED)
#target_link_libraries(RPiDrone RPiComponents)
#target_link_libraries(RPiDrone RPiSocket)
#target_link_libraries(RPiDrone Controllers)

#System libs
find_library(wiringPi REQUIRED)
find_library(wiringPiDev REQUIRED)
find_library(pthread REQUIRED)
find_library(bluetooth REQUIRED)
target_link_libraries(${PROJECT_NAME} bluetooth)
target_link_libraries(${PROJECT_NAME} wiringPi)
target_link_libraries(${PROJECT_NAME} wiringPiDev)
target_link_libraries(${PROJECT_NAME} pthread)