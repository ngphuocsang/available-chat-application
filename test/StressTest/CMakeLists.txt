cmake_minimum_required(VERSION 3.8.0)

set(MYLIBRARY_INCLUDE_DIR ../../libraries/MyLibrary)
set(CLIENT_INCLUDE_DIR ../../source/Client)
set(SERVER_INCLUDE_DIR ./.../source/Server)
set(cpprestsdk_DIR /usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}/cmake/)
set(INCLUDE_DIR
    ${MYLIBRARY_INCLUDE_DIR}
    ${MYLIBRARY_INCLUDE_DIR}/old
    ${MYLIBRARY_INCLUDE_DIR}/Models
    ${SERVER_INCLUDE_DIR}/Database
    ${SERVER_INCLUDE_DIR}/ChatServer/Headers
    ${SERVER_INCLUDE_DIR}/RestServer
    ${SERVER_INCLUDE_DIR}/RestServer/Controllers
    ${SERVER_INCLUDE_DIR}/RestServer/Services
    ${CLIENT_INCLUDE_DIR}/ChatApp
    ${CLIENT_INCLUDE_DIR}/RestAPI
    ../../test
)
set(LINK_LIBRARIES
    cpprestsdk::cpprest
    Boost::system
    pthread
    gtest
    mysqlclient
    MyLibrary
    ACA-Client-lib
    ACA-Database-lib
    ACA-ChatServer-lib
    ACA-RestServer-lib
)

find_package(GTest REQUIRED)
find_package(cpprestsdk REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)

file(GLOB files "test_*.cpp")

foreach(file ${files})
	string(REGEX REPLACE "(^.*/|\\.[^.]*$)" "" file_without_ext ${file})
	add_executable(${file_without_ext} ${file})
    target_include_directories(${file_without_ext} PUBLIC
        ${INCLUDE_DIR}
    )
	target_link_libraries(${file_without_ext} PRIVATE
        ${LINK_LIBRARIES}
    )
	add_test(${file_without_ext} ${file_without_ext})
endforeach()