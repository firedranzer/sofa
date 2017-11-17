cmake_minimum_required(VERSION 3.1)
project(SofaAllComponents)

set(PROJECT_VERSION 1.0)
set(SOURCE_FILES init.cpp)

find_package(Sofa REQUIRED)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} )
target_link_libraries(${PROJECT_NAME} SofaCore)
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DSOFA_BUILD_SOFALLCOMMONCOMPONENTS -DSOFA_SRC_DIR=\"${CMAKE_SOURCE_DIR}\"")
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")



