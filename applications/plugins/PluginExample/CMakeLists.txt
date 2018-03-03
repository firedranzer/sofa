cmake_minimum_required(VERSION 3.1)
project(PluginExample)

set(HEADER_FILES
    include/PluginExample/MyBehaviorModel.h
    include/PluginExample/MyMappingPendulumInPlane.h
    include/PluginExample/MyMappingPendulumInPlane.inl
    include/PluginExample/MyProjectiveConstraintSet.h
    include/PluginExample/MyProjectiveConstraintSet.inl
    include/PluginExample/config.h
)

set(HEADER_FILES_TO_MOC
    include/PluginExample/MyDataWidgetUnsigned.h
)

set(SOURCE_FILES
    src/MyBehaviorModel.cpp
    src/MyDataWidgetUnsigned.cpp
    src/MyMappingPendulumInPlane.cpp
    src/MyProjectiveConstraintSet.cpp
    initPlugin.cpp
)

set(README_FILES PluginExample.txt)
set(EXAMPLE_FILES examples/PluginExampleTest.scn)

find_package(SofaGui REQUIRED)

find_package(Qt5 COMPONENTS Core QUIET)
if(Qt5Core_FOUND)
    qt5_wrap_cpp(MOCCED_HEADER_FILES ${HEADER_FILES_TO_MOC})
else()
    message(ERROR "Qt5 not found.s")
endif()

if(SOFA_BUILD_TESTS)
    add_subdirectory(tests)
endif()

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES_TO_MOC} ${MOCCED_HEADER_FILES} ${HEADER_FILES} ${SOURCE_FILES} ${README_FILES} ${EXAMPLE_FILES})

### You can set the visibility of this flag to public to allow other packages to have it defined.
target_compile_definitions(${PROJECT_NAME} PRIVATE "-DSOFA_BUILD_PLUGINEXAMPLE")
target_link_libraries(${PROJECT_NAME} SofaGuiQt)

### You can set the visibility of the include directory to public to allow other packages to be able to
### #include<PluginExample/>.
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/)

install(TARGETS ${PROJECT_NAME}
        COMPONENT PluginExample_libraries
        EXPORT PluginExampleTargets
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
