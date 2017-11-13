cmake_minimum_required(VERSION 3.1)
project(SofaImplicitField)

set(SOFAIMPLICITFIELD_VERSION 1.0)
set(SOFA_HAVE_SOFAIMPLICITFIELD 1)

find_package(SofaDistanceGrid REQUIRED)

set(HEADER_FILES
    config.h
    initSofaImplicitField.h

    # This is backward compatibility
    deprecated/SphereSurface.h
    deprecated/ImplicitSurfaceContainer.h    # This is a backward compatibility file toward ScalarField
    deprecated/InterpolatedImplicitSurface.h # This is a backward compatibility file toward DiscreteridField

    components/geometry/DiscreteGridField.h
    components/geometry/SphericalField.h
    components/geometry/ScalarField.h
    components/geometry/SurfaceMeshGenerationFromScalarField.h
    components/mapping/ImplicitSurfaceMapping.h
    components/mapping/ImplicitSurfaceMapping.inl
    components/visual/PointCloudImplicitFieldVisualization.h
    components/visual/ImplicitFieldShaderVisualization.h
    components/visual/ImplicitFieldShaderVisualization.inl
)

set(SOURCE_FILES
    initSofaImplicitField.cpp

    ## This is a backward compatibility..
    deprecated/SphereSurface.cpp
    deprecated/InterpolatedImplicitSurface.cpp

    components/geometry/ScalarField.cpp
    components/geometry/DiscreteGridField.cpp
    components/geometry/SphericalField.cpp
    components/geometry/SurfaceMeshGenerationFromScalarField.cpp
    components/mapping/ImplicitSurfaceMapping.cpp
    components/visual/PointCloudImplicitFieldVisualization.cpp
    components/visual/ImplicitFieldShaderVisualization.cpp
)

find_package(SofaPython QUIET)
if(SofaPython_FOUND)
    list(APPEND HEADER_FILES
        components/geometry/CustomField.h)

    list(APPEND SOURCE_FILES
        components/geometry/CustomField.cpp)

    add_definitions("-DDO_SOFAPYTHON_FEATURES=1")
else()
    message(STATUS "SofaImplicitField: could not find SofaPython, won't build the component(s): <CustomField>.")
    add_definitions("-DDO_SOFAPYTHON_FEATURES=0")
endif()

set(EXTRA_FILES
    README.md
    examples/README.md
    examples/CustomField.scn
    examples/CustomFieldVisualization.scn
    examples/CustomShaderField.scn
    examples/customfield.py
    examples/cython/CustomField.scn
    examples/cython/customfield.py
    examples/cython/cythonfield.pyx
    )

add_subdirectory(SofaImplicitField_test)

configure_file(config.h.in "${CMAKE_BINARY_DIR}/include/SofaImplicitField/config.h")
add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES} ${EXTRA_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DSOFA_HAVE_SOFAIMPLICITFIELD")
target_link_libraries(${PROJECT_NAME} PUBLIC SofaDistanceGrid)

if(SofaPython_FOUND)
    target_link_libraries(${PROJECT_NAME} PUBLIC SofaPython)
endif()

# Config files and install rules for pythons scripts
sofa_set_python_directory(${PROJECT_NAME} "python")


target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>")
target_include_directories(${PROJECT_NAME} PUBLIC "$<INSTALL_INTERFACE:include>")

## Install rules for the library and headers; CMake package configurations files
sofa_create_package(SofaImplicitField ${SOFAIMPLICITFIELD_VERSION} ${PROJECT_NAME} SofaImplicitField)
