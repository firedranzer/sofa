cmake_minimum_required(VERSION 3.1)

# Force cmake to use the NEW policy regaring the "VERSION" management.
cmake_policy(SET CMP0048 NEW)

project(Sofa.Core.Async VERSION 1.0)

find_package(SofaFramework REQUIRED)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

set(HEADER_FILES
        initPlugin.h
        config.h
        ComponentTracker.h
    )
set(SOURCE_FILES
        initPlugin.cpp
        ComponentTracker.cpp
    )

set(README_FILES README.md)

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${MOC_FILES} ${SOURCE_FILES} ${README_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")

target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>")
target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>")
target_include_directories(${PROJECT_NAME} PUBLIC "$<INSTALL_INTERFACE:include>")

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DSOFA_BUILD_CORE_ASYNC")
target_link_libraries(${PROJECT_NAME} SofaCore)

## Install rules for the library and headers; CMake package configurations files
sofa_create_package(Sofa.Core.Async ${PROJECT_VERSION} ${PROJECT_NAME} Sofa.Core.Async)

if(SOFA_BUILD_TESTS)
    add_subdirectory(tests)
endif()


