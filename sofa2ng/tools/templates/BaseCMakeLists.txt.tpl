#encoding utf-8
#compiler-settings
commentStartToken = //
cheetahVarStartToken = autopack::
#end compiler-settings

### CMakeLists.txt are generated from our lovely sofa-pck-manager tools 
### In case you need to customize this CMakeLists.txt please copy our template from
### autopack::{cmake_template_src_path}
### then launch sofa-pck-manager regenerate CMakeLists.txt
cmake_minimum_required(VERSION 3.1)
project(autopack::package_name)

#for depend in autopack::dependencies
find_package(autopack::depend REQUIRED)
#end for

set(HEADER_FILES
#for filename in autopack::header_files   
    autopack::filename
#end for
)

set(SOURCE_FILES
#for filename in autopack::source_files 
    autopack::filename
#end for
)

set(EXTRA_FILES
#for filename in autopack::extra_files   
    autopack::filename
#end for
)

#if autopack::package_type == "executable"
add_autopack::{package_type}(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${EXTRA_FILES})
#else
add_autopack::{package_type}(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES} ${EXTRA_FILES})
#end if
target_link_libraries(${PROJECT_NAME} PUBLIC autopack::{sorted_dependencies} )
target_compile_definitions(${PROJECT_NAME} PRIVATE "-DBUILD_TARGET_autopack::package_cname")
target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/deprecated_layout>")

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_BINARY_DIR} EXPORT Find${PROJECT_NAME})
export(TARGETS ${PROJECT_NAME} FILE "${CMAKE_BINARY_DIR}/cmake/Find${PROJECT_NAME}.cmake")


