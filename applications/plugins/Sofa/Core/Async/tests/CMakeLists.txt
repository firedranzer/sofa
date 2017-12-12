cmake_minimum_required(VERSION 3.1)

project(Sofa.Core.Async_test)

set(SOURCE_FILES
    ComponentTracker_test.cpp
    AsyncComponentTracker_test.cpp
)

find_package(Sofa.Core.Async REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} SofaGTestMain Sofa.Core.Async)
target_include_directories(${PROJECT_NAME} PUBLIC ${Sofa.Core.Async_INCLUDE_DIRS})

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

