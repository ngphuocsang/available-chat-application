cmake_minimum_required(VERSION 3.10.0)
project(MyLibrary VERSION 0.1.0)

set(cpprestsdk_DIR /usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}/cmake/)
set(INCLUDE_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Models
    ${CMAKE_CURRENT_SOURCE_DIR}/RestAPI
    ${CMAKE_CURRENT_SOURCE_DIR}/UserInterfaces
)
set(LINK_LIBRARIES
    cpprestsdk::cpprest
    cryptopp
    Boost::system
)

find_package(cpprestsdk REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)

file(GLOB SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Models/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Models/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/RestAPI/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/RestAPI/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/UserInterfaces/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/UserInterfaces/*.cpp"
)

add_library(MyLibrary STATIC ${SOURCE_FILES})
target_include_directories(MyLibrary PUBLIC ${INCLUDE_DIR})
target_link_libraries(MyLibrary PRIVATE ${LINK_LIBRARIES})