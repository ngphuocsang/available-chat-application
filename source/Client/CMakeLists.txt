cmake_minimum_required(VERSION 3.10.0)
project(ACA-Client VERSION 0.1.0)

set(MYLIBRARY_INCLUDE_DIR ../../libraries/MyLibrary)
set(cpprestsdk_DIR /usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}/cmake/)
set(INCLUDE_DIR
    ${MYLIBRARY_INCLUDE_DIR}
    ${MYLIBRARY_INCLUDE_DIR}/Models
    ${MYLIBRARY_INCLUDE_DIR}/UserInterfaces
    ${MYLIBRARY_INCLUDE_DIR}/RestAPI
    ${CMAKE_CURRENT_SOURCE_DIR}/ChatApp
    ${CMAKE_CURRENT_SOURCE_DIR}/UI
    ${CMAKE_CURRENT_SOURCE_DIR}/Text
)
set(LINK_LIBRARIES
    cpprestsdk::cpprest
    Boost::system
    MyLibrary
    Qt5::Widgets
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(cpprestsdk REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

file(GLOB SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/Text/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Text/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ChatApp/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/ChatApp/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/UI/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/UI/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/UI/*.ui"
)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/chat_history)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/resources)

add_library(ACA-Client-lib STATIC ${SOURCE_FILES})
target_include_directories(ACA-Client-lib PUBLIC ${INCLUDE_DIR})
target_link_libraries(ACA-Client-lib PRIVATE ${LINK_LIBRARIES})


add_executable(ACA-Client-CLI mainCLI.cpp)
target_include_directories(ACA-Client-CLI PUBLIC ${INCLUDE_DIR})
target_link_libraries(ACA-Client-CLI PRIVATE ${LINK_LIBRARIES} ACA-Client-lib)

add_executable(ACA-Client-GUI mainGUI.cpp)
target_include_directories(ACA-Client-GUI PUBLIC ${INCLUDE_DIR})
target_link_libraries(ACA-Client-GUI PRIVATE ${LINK_LIBRARIES} ACA-Client-lib)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/chat_history)