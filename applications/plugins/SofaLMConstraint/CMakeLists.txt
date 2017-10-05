cmake_minimum_required(VERSION 3.1)
project(SofaLMConstraint)

set(SOFALMCONSTRAINT_VERSION 1.0)
set(SOFA_HAVE_SOFALMCONSTRAINT 1)


################################ COMPONENTS HERE ARE THE NG-SET ####################################
set(HEADER_FILES
    config.h
    initLMConstraint.h
    )

set(SOURCE_FILES
    initLMConstraint.cpp
    )

############################## COMPONENTS HERE ARE THE LIGHT-SET ###################################
if(SOFA_BUILD_COMPONENTSET_LIGHT)
    list(APPEND HEADER_FILES

        )
    list(APPEND SOURCE_FILES

        )
endif()

############################## COMPONENTS HERE ARE THE FULL-SET ####################################
if(SOFA_BUILD_COMPONENTSET_STANDARD)
    list(APPEND HEADER_FILES
        BarycentricDistanceLMConstraintContact.h
        BarycentricDistanceLMConstraintContact.inl
        DistanceLMContactConstraint.h
        DistanceLMContactConstraint.inl
        DistanceLMConstraint.h
        DistanceLMConstraint.inl
        FixedLMConstraint.h
        FixedLMConstraint.inl
        LMConstraintSolver.h
        LMConstraintDirectSolver.h
        DOFBlockerLMConstraint.h
        DOFBlockerLMConstraint.inl
        )

    list(APPEND SOURCE_FILES
        BarycentricDistanceLMConstraintContact.cpp
        DOFBlockerLMConstraint.cpp
        FixedLMConstraint.cpp
        DistanceLMContactConstraint.cpp
        DistanceLMConstraint.cpp
        LMConstraintSolver.cpp
        LMConstraintDirectSolver.cpp
        #TetrahedronBarycentricDistanceLMConstraintContact.cpp
        )

    if(SOFA_HAVE_MINIFLOWVR)
        list(APPEND SOURCE_FILES BarycentricDistanceLMConstraintContact_DistanceGrid.cpp)
    endif()
endif()

############################### COMPONENTS HERE ARE DEPRECATED ####################################
if(SOFA_BUILD_COMPONENTSET_FULL)

endif()

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
#target_link_libraries(${PROJECT_NAME} PUBLIC SofaBaseTopology SofaGeneralMeshCollision SofaGeneralDeformable SofaConstraint SofaMeshCollision SofaExplicitOdeSolver SofaGeneralExplicitOdeSolver SofaImplicitOdeSolver SofaBaseCollision)
target_link_libraries(${PROJECT_NAME} PUBLIC SofaMeshCollision SofaSimpleFem SofaImplicitOdeSolver SofaUserInteraction SofaBaseLinearSolver)
target_link_libraries(${PROJECT_NAME} PUBLIC SofaEigen2Solver)

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DSOFA_BUILD_LM_CONSTRAINT")
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")

target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>")
sofa_create_package(SofaLMConstraint ${SOFALMCONSTRAINT_VERSION} ${PROJECT_NAME} SofaLMConstraint)
