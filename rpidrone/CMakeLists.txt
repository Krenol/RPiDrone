project (rpidrone)

#set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(RPISOCKET_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/RPiSocket CACHE INTERNAL "Path to include folder for RPiSocket")
set(JSON_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/json CACHE INTERNAL "Path to include folder for JSON")
#set(RPICOMP_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/RPiComponents CACHE INTERNAL "Path to include folder for RPiComponents")
#set(CONTROLLERS_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/CppControllers CACHE INTERNAL "Path to include folder for CppControllers")
set(DESIGN_PATTERNS_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/CppDesignPatterns CACHE INTERNAL "Path to include folder for CppDesignPatterns")
#set(UTILS_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/CppUtils CACHE INTERNAL "Path to include folder for CppUtils")

set(RPICDRONE_PROJECT_SRCS 
${PROJECT_SOURCE_DIR}/loop.cpp
${PROJECT_SOURCE_DIR}/main.cpp
${PROJECT_SOURCE_DIR}/logs/easylogging++.cc
${PROJECT_SOURCE_DIR}/parsers/json_config_parser.cpp
${PROJECT_SOURCE_DIR}/parsers/json_input_parser.cpp
${PROJECT_SOURCE_DIR}/parsers/arduino_config_parser.cpp
${PROJECT_SOURCE_DIR}/parsers/arduino_input_parser.cpp
${PROJECT_SOURCE_DIR}/parsers/arduino_output_parser.cpp
${PROJECT_SOURCE_DIR}/connection.cpp
${PROJECT_SOURCE_DIR}/wiringSerial.c
${PROJECT_SOURCE_DIR}/arduino.cpp
)

include_directories(${PROJECT_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${RPISOCKET_INCLUDE_DIR})
include_directories(${JSON_INCLUDE_DIR})
#include_directories(${RPICOMP_INCLUDE_DIR})
#include_directories(${CONTROLLERS_INCLUDE_DIR})
include_directories(${DESIGN_PATTERNS_INCLUDE_DIR})
#include_directories(${UTILS_INCLUDE_DIR})


if (NOT TARGET rpisocket)
add_subdirectory (external/RPiSocket)
endif ()


if (NOT TARGET rpicomponents)
    add_subdirectory (external/RPiPins)
endif ()
if (NOT TARGET design_patterns)
    add_subdirectory (external/CppDesignPatterns)
endif ()

if (NOT TARGET nlohmann_json::nlohmann_json)
    add_subdirectory (external/json EXCLUDE_FROM_ALL)
endif ()



add_executable (rpidrone ${RPICDRONE_PROJECT_SRCS})

# Own libs

find_library(rpipins REQUIRED)
find_library(rpisocket REQUIRED)
find_library(design_patterns REQUIRED)
target_link_libraries(${PROJECT_NAME} rpisocket)
target_link_libraries(${PROJECT_NAME} rpipins)
target_link_libraries(${PROJECT_NAME} design_patterns)

#System libs
find_library(pigpio REQUIRED)
find_library(pthread REQUIRED)
find_library(bluetooth REQUIRED)
find_library(atomic REQUIRED)
target_link_libraries(${PROJECT_NAME} bluetooth)
target_link_libraries(${PROJECT_NAME} pigpio)
target_link_libraries(${PROJECT_NAME} pthread)
target_link_libraries(${PROJECT_NAME} atomic)
target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)