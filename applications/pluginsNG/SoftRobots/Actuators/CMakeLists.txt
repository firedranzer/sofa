cmake_minimum_required(VERSION 3.1)
project(SoftRobots.Actuators)

set(SOFA_VERSION 1.0)

set(SOURCE_FILES
    init.cpp
)

find_package(Sofa.Mechanics REQUIRED)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} )
target_link_libraries(${PROJECT_NAME} SofaComponentCommon)
target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>")
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DSOFA_BUILD_SOFALLCOMMONCOMPONENTS -DSOFA_SRC_DIR=\"${CMAKE_SOURCE_DIR}\"")
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")

## Install rules for the library and headers; CMake package configurations files
#sofa_create_package(Sofa.Common ${SOFA_VERSION} ${PROJECT_NAME} Sofa.Common)

