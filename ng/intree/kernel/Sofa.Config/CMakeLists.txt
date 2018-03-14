cmake_minimum_required(VERSION 3.5)


### CMakeLists.txt are generated from our lovely sofa-pck-manager (spm) tools 
### In case you need to customize this CMakeLists.txt please copy it from
### sofa2ng/recipies/Sofa.Config/CMakeLists.txt.tpl
### And use the copy in your recipe. 
project(Sofa.Config VERSION 1.0 LANGUAGES CXX)

set(SOURCE_FILES
    src/sofa/config/basemacros.cpp
)

set(HEADER_FILES
    src/sofa/config/basemacros.h
)

set(SOFA_BUILD_OPTIONS_TEMPLATES
    config/build_option_componentset.h
    config/build_option_dump_visitor.h
    config/build_option_experimental_features.h
    config/build_option_opengl.h
    config/build_option_threading.h
    config/sharedlibrary_defines.h
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
