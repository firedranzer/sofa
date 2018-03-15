
### CMakeLists.txt are generated from our lovely *spm* tool using the following template:
### /home/dmarchal/projects/2016/DEFROST/04-code/sofa-src/sofa/master/forkdefrost/sofa/sofa2ng/tools/templates/BaseCMakeLists.txt.tpl
### In case you need to customize this CMakeLists.txt please copy this template 
### and pass the path to your version in the cmake_template property of the project.

cmake_minimum_required(VERSION 3.1)
project(Sofa.Component.Utils VERSION 1.0 LANGUAGES CXX)

find_package(Sofa.Core QUIET)

set(ALL_DEPENDENCIES_FOUND 1)
if(NOT Sofa.Core_FOUND)
    set(ALL_DEPENDENCIES_FOUND)
endif()

if(ALL_DEPENDENCIES_FOUND)

set(HEADER_FILES
    src/InfoComponent.h
    src/MakeAliasComponent.h
    src/MakeDataAliasComponent.h
    src/MessageHandlerComponent.h
)

set(SOURCE_FILES
    src/InfoComponent.cpp
    src/MakeAliasComponent.cpp
    src/MakeDataAliasComponent.cpp
    src/MessageHandlerComponent.cpp
)

set(EXTRA_FILES
)

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES} ${EXTRA_FILES})
target_link_libraries(${PROJECT_NAME} PUBLIC  Sofa.Core)
target_compile_definitions(${PROJECT_NAME} PRIVATE "-DBUILD_TARGET_SOFA_COMPONENT_UTILS")
target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/deprecated_layout>")

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_BINARY_DIR} EXPORT Find${PROJECT_NAME})
export(TARGETS ${PROJECT_NAME} FILE "${CMAKE_BINARY_DIR}/cmake/Find${PROJECT_NAME}.cmake")
else()
    message(WARNING "There is missing dependencies. The Sofa.Component.Utils is then disabled")
endif()
