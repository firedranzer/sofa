cmake_minimum_required(VERSION 3.1)
project(SofaImplicitField)

set(SOFAIMPLICITFIELD_VERSION 1.0)
set(SOFA_HAVE_SOFAIMPLICITFIELD 1)

find_package(SofaDistanceGrid REQUIRED)

set(HEADER_FILES
    config.h
    initSofaImplicitField.h
    components/geometry/DiscreteGridField.h
    components/geometry/SphericalField.h
    components/geometry/ScalarField.h
    components/geometry/ImplicitSurfaceContainer.h
    components/geometry/ImplicitSurfaceMapping.h
    components/geometry/ImplicitSurfaceMapping.inl
    components/geometry/InterpolatedImplicitSurface.h
    components/geometry/InterpolatedImplicitSurface.inl
)

set(SOURCE_FILES
    initSofaImplicitField.cpp
    components/geometry/DiscreteGridField.cpp
    components/geometry/SphericalField.cpp
    components/geometry/ImplicitSurfaceContainer.cpp
    components/geometry/ImplicitSurfaceMapping.cpp
    components/geometry/InterpolatedImplicitSurface.cpp
)

set(EXTRA_FILES
    README.md
    examples/README.md
    )


add_subdirectory(SofaImplicitField_test)

configure_file(config.h.in "${CMAKE_BINARY_DIR}/include/SofaImplicitField/config.h")
add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES} ${EXTRA_FILES})
target_link_libraries(${PROJECT_NAME} PUBLIC SofaDistanceGrid)
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")

target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>")
target_include_directories(${PROJECT_NAME} PUBLIC "$<INSTALL_INTERFACE:include>")


## Install rules for the library and headers; CMake package configurations files
sofa_create_package(SofaImplicitField ${SOFAIMPLICITFIELD_VERSION} ${PROJECT_NAME} SofaImplicitField)
