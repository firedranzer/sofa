#encoding utf-8
#compiler-settings
commentStartToken = //
cheetahVarStartToken = autopack::
#end compiler-settings
cmake_minimum_required(VERSION 3.5)


### CMakeLists.txt are generated from our lovely sofa-pck-manager (spm) tools 
### In case you need to customize this CMakeLists.txt please copy it from
### autopack::{cmake_template_src_path}
### And use the copy in your recipe. 
project(autopack::package_name VERSION 1.0 LANGUAGES CXX)

set(SOURCE_FILES
#for filename in autopack::source_files 
    autopack::filename
#end for
)

set(HEADER_FILES
#for filename in autopack::header_files 
    autopack::filename
#end for
)

set(SOFA_BUILD_OPTIONS_TEMPLATES
#for filename in autopack::build_options_templates
    autopack::filename
#end for
)

foreach(NAME ${SOFA_BUILD_OPTIONS_TEMPLATES})
    configure_file("templates/${NAME}.in" "${CMAKE_BINARY_DIR}/include/sofa/${NAME}")
    install(FILES "${CMAKE_BINARY_DIR}/include/sofa/${NAME}" DESTINATION "include/sofa/config")
endforeach()

configure_file("templates/config.h.in" "${CMAKE_BINARY_DIR}/include/sofa/config.h")
install(FILES "${CMAKE_BINARY_DIR}/include/sofa/config.h" DESTINATION "include/sofa/")

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>")
target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")

install(FILES "${CMAKE_BINARY_DIR}/include/sofa/config.h" DESTINATION "include/sofa/")
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_BINARY_DIR} EXPORT FindSofa.Config)
export(TARGETS ${PROJECT_NAME} FILE "${CMAKE_BINARY_DIR}/cmake/Find${PROJECT_NAME}.cmake")
