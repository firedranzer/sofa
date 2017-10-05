cmake_minimum_required(VERSION 3.1)
project(SofaDenseSolver)
set(SOFADENSESOLVER_VERSION 1.0)

# Dependencies
find_package(Newmat)

################################ COMPONENTS HERE ARE THE NG-SET ####################################
set(HEADER_FILES
    SofaDenseSolver.h
    initSofaDenseSolver.h
)

set(SOURCE_FILES
    initSofaDenseSolver.cpp
)

############################## COMPONENTS HERE ARE THE LIGHT-SET ###################################
if(SOFA_BUILD_COMPONENTSET_LIGHT)
    list(APPEND HEADER_FILES

        )
    list(APPEND SOURCE_FILES

        )
endif()

############################## COMPONENTS HERE ARE THE STANDARD-SET ################################
if(SOFA_BUILD_COMPONENTSET_STANDARD)
    list(APPEND HEADER_FILES
        LULinearSolver.h
        )
    list(APPEND SOURCE_FILES
        LULinearSolver.cpp
        )
    if(${Newmat_FOUND})
        list(APPEND HEADER_FILES
            NewMatMatrix.h
            NewMatVector.h
            )
        list(APPEND SOURCE_FILES
            NewMatCGLinearSolver.cpp
            NewMatCholeskySolver.cpp
            )
    endif()
endif()

############################### COMPONENTS HERE ARE DEPRECATED ####################################
if(SOFA_BUILD_COMPONENTSET_FULL)
endif()


add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>")
target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>")
target_link_libraries(${PROJECT_NAME} PUBLIC SofaBaseLinearSolver SofaGeneralLinearSolver)
if(${Newmat_FOUND})
    target_link_libraries(${PROJECT_NAME} PUBLIC newmat)
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DSOFA_BUILD_DENSE_SOLVER")
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")

sofa_create_package(SofaDenseSolver ${SOFADENSESOLVER_VERSION} ${PROJECT_NAME} SofaDenseSolver)
