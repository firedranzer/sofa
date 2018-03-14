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

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${EXTRA_FILES})
target_link_libraries(${PROJECT_NAME} PUBLIC autopack::{sorted_dependencies} )
target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_BINARY_DIR} EXPORT Find${PROJECT_NAME})


