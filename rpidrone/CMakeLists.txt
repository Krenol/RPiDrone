# CMakeList.txt : CMake project for RPiComponents, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.4)
set(CMAKE_CXX_STANDARD 14)  # enable C++14 standard

project (RPiDrone)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
 
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
 
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(RPISOCKET_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/RPiSocket CACHE INTERNAL "Path to include folder for RPiSocket")
set(RPICOMP_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/RPiComponents CACHE INTERNAL "Path to include folder for RPiComponents")
set(CONTROLLERS_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/CppControllers CACHE INTERNAL "Path to include folder for CppControllers")
set(DESIGN_PATTERNS_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/CppDesignPatterns CACHE INTERNAL "Path to include folder for CppDesignPatterns")
set(UTILS_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/CppUtils CACHE INTERNAL "Path to include folder for CppUtils")

set(RPICDRONE_PROJECT_SRCS 
${PROJECT_SOURCE_DIR}/main.cpp
${PROJECT_SOURCE_DIR}/sensorics.cpp
)

include_directories(${PROJECT_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${RPISOCKET_INCLUDE_DIR})
include_directories(${RPICOMP_INCLUDE_DIR})
include_directories(${CONTROLLERS_INCLUDE_DIR})
include_directories(${DESIGN_PATTERNS_INCLUDE_DIR})
include_directories(${UTILS_INCLUDE_DIR})

add_subdirectory (external/RPiSocket/rpisocket)
add_subdirectory (external/RPiComponents/rpicomponents)
add_subdirectory (external/CppControllers/controllers)
add_subdirectory (external/CppDesignPatterns/design_patterns)
add_subdirectory (external/CppUtils/utils)

add_executable (RPiDrone ${RPICDRONE_PROJECT_SRCS})

# Own libs
find_library(RPiComponents REQUIRED)
find_library(RPiPins REQUIRED)
find_library(RPiSocket REQUIRED)
find_library(Controllers REQUIRED)
find_library(design_patterns REQUIRED)
find_library(utils REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE RPiComponents)
target_link_libraries(${PROJECT_NAME} PRIVATE RPiSocket)
target_link_libraries(${PROJECT_NAME} PRIVATE RPiPins)
target_link_libraries(${PROJECT_NAME} PRIVATE Controllers)
target_link_libraries(${PROJECT_NAME} PRIVATE design_patterns)
target_link_libraries(${PROJECT_NAME} PRIVATE utils)

#System libs
find_library(wiringPi REQUIRED)
find_library(wiringPiDev REQUIRED)
find_library(pthread REQUIRED)
find_library(bluetooth REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE bluetooth)
target_link_libraries(${PROJECT_NAME} PRIVATE wiringPi)
target_link_libraries(${PROJECT_NAME} PRIVATE wiringPiDev)
target_link_libraries(${PROJECT_NAME} PRIVATE pthread)