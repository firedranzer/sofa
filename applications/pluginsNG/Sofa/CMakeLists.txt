cmake_minimum_required(VERSION 3.1)
project(Sofa)

set(PROJECT_VERSION 1.0)

set(SOURCE_FILES
    init.cpp
    test1.scn
)

sofa_add_plugin(Haptics Sofa.Haptics ON)
sofa_add_plugin(Mechanics Sofa.Mechanics ON)
sofa_add_plugin(Loader Sofa.Loader ON)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} )
target_link_libraries(${PROJECT_NAME} SofaCore)
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DSOFA_BUILD_SOFALLCOMMONCOMPONENTS -DSOFA_SRC_DIR=\"${CMAKE_SOURCE_DIR}\"")
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")


#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/..")

## Install rules for the library and headers; CMake package configurations files
sofa_create_package(Sofa ${PROJECT_VERSION} ${PROJECT_NAME} Sofa)
