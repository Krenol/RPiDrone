project (rpidrone)

set(PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(JSON_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/json CACHE INTERNAL "Path to include folder for JSON")
set(DESIGN_PATTERNS_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/CppDesignPatterns CACHE INTERNAL "Path to include folder for CppDesignPatterns")
set(WEBSOCKET_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/Simple-WebSocket-Server CACHE INTERNAL "Path to include folder for websocketpp")

set(RPICDRONE_PROJECT_SRCS 
${PROJECT_SOURCE_DIR}/connected_client_handler.cpp
${PROJECT_SOURCE_DIR}/main.cpp
${PROJECT_SOURCE_DIR}/logs/easylogging++.cc
${PROJECT_SOURCE_DIR}/parsers/json/config_parser.cpp
${PROJECT_SOURCE_DIR}/parsers/json/client_input_parser.cpp
${PROJECT_SOURCE_DIR}/parsers/flightcontroller/config_parser.cpp
${PROJECT_SOURCE_DIR}/parsers/flightcontroller/input_parser.cpp
${PROJECT_SOURCE_DIR}/parsers/flightcontroller/output_parser.cpp
${PROJECT_SOURCE_DIR}/wiringSerial.c
${PROJECT_SOURCE_DIR}/arduino.cpp
${PROJECT_SOURCE_DIR}/websocket.cpp
${PROJECT_SOURCE_DIR}/config.cpp
${PROJECT_SOURCE_DIR}/api/api_server.cpp
${PROJECT_SOURCE_DIR}/middleware/client.cpp
${PROJECT_SOURCE_DIR}/middleware/flightcontroller.cpp
)

include_directories(${PROJECT_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${JSON_INCLUDE_DIR})
include_directories(${DESIGN_PATTERNS_INCLUDE_DIR})

if (NOT TARGET Simple-WebSocket-Server)
    add_subdirectory (external/Simple-WebSocket-Server)
endif ()

if (NOT TARGET design_patterns)
    add_subdirectory (external/CppDesignPatterns)
endif ()

if (NOT TARGET nlohmann_json::nlohmann_json)
    add_subdirectory (external/json EXCLUDE_FROM_ALL)
endif ()

add_executable (rpidrone ${RPICDRONE_PROJECT_SRCS})

# Own libs
find_library(design_patterns REQUIRED)
target_link_libraries(${PROJECT_NAME} design_patterns)

#System libs
find_library(pigpio REQUIRED)
find_library(pthread REQUIRED)
find_library(bluetooth REQUIRED)
find_library(atomic REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost 1.54.0 COMPONENTS system thread coroutine context REQUIRED)
target_link_libraries(${PROJECT_NAME} bluetooth)
target_link_libraries(${PROJECT_NAME} pigpio)
target_link_libraries(${PROJECT_NAME} pthread)
target_link_libraries(${PROJECT_NAME} atomic)
target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
target_include_directories(${PROJECT_NAME} INTERFACE ${Boost_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${OPENSSL_LIBRARIES})
target_include_directories(${PROJECT_NAME} INTERFACE ${OPENSSL_INCLUDE_DIR})