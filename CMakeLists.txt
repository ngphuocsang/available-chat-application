cmake_minimum_required(VERSION 3.10.0)
project(ACA VERSION 0.1.0)

enable_testing()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${GTEST_INCLUDE_DIRS})

add_subdirectory(libraries/MyLibrary)
add_subdirectory(source/Client)
add_subdirectory(source/Server)
add_subdirectory(test)

file(
    COPY ${CMAKE_CURRENT_SOURCE_DIR}/libraries/MyLibrary/UserInterfaces/logo.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/source/Client
)

file(
    COPY ${CMAKE_CURRENT_SOURCE_DIR}/source/Client/resources
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/source/Client
)