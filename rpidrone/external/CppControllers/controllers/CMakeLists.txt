project (controllers VERSION 0.0.42)
 
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

set(CONTROLLERS_PROJECT_SRCS 
${PROJECT_SOURCE_DIR}/controller.cpp
${PROJECT_SOURCE_DIR}/p.cpp
${PROJECT_SOURCE_DIR}/pd.cpp
${PROJECT_SOURCE_DIR}/pi.cpp
${PROJECT_SOURCE_DIR}/pid.cpp
${PROJECT_SOURCE_DIR}/pid_aw.cpp
)

add_library(${PROJECT_NAME} SHARED ${CONTROLLERS_PROJECT_SRCS})

target_include_directories(${PROJECT_NAME}
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${PROJECT_INCLUDE_DIR}>
    PRIVATE
        ${PROJECT_SOURCE_DIR}
)
find_package(Eigen3 REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC Eigen3::Eigen)
target_link_libraries(${PROJECT_NAME} PRIVATE pthread)

if(${BUILD_TEST})
    add_subdirectory(test)
endif()
