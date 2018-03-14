
### CMakeLists.txt are generated from our lovely sofa-pck-manager tools 
### In case you need to customize this CMakeLists.txt please copy our template from
### /home/dmarchal/projects/2016/DEFROST/04-code/sofa-src/sofa/master/forkdefrost/sofa/sofa2ng/tools/templates/TestCMakeLists.txt.tpl
### then launch sofa-pck-manager regenerate CMakeLists.txt
cmake_minimum_required(VERSION 3.1)
project(Sofa.Config_tests)

find_package(Sofa.Config REQUIRED)

set(HEADER_FILES
)

set(SOURCE_FILES
    src/main.cpp
)

set(EXTRA_FILES
)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${EXTRA_FILES})
target_link_libraries(${PROJECT_NAME} PUBLIC  Sofa.Config )
target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_BINARY_DIR} EXPORT Find${PROJECT_NAME})


