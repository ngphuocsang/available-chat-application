cmake_minimum_required(VERSION 3.10.0)
project(ACA-Server VERSION 0.1.0)

set(MYLIBRARY_INCLUDE_DIR ../../libraries/MyLibrary)
set(cpprestsdk_DIR /usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}/cmake/)
set(INCLUDE_DIR
    ${MYLIBRARY_INCLUDE_DIR}
    ${MYLIBRARY_INCLUDE_DIR}/old
    ${MYLIBRARY_INCLUDE_DIR}/Models
    ${CMAKE_CURRENT_SOURCE_DIR}/Database
    ${CMAKE_CURRENT_SOURCE_DIR}/ChatServer/Headers
    ${CMAKE_CURRENT_SOURCE_DIR}/RestServer
    ${CMAKE_CURRENT_SOURCE_DIR}/RestServer/Controllers
    ${CMAKE_CURRENT_SOURCE_DIR}/RestServer/Services
)
set(LINK_LIBRARIES
    cpprestsdk::cpprest
    Boost::system
    mysqlclient
    MyLibrary
)

find_package(cpprestsdk REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)

file(GLOB SOURCE_FILES_DATABASE 
    ${CMAKE_CURRENT_SOURCE_DIR}/Database/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Database/*.cpp
)

file(GLOB SOURCE_FILES_CHATSERV
    ${CMAKE_CURRENT_SOURCE_DIR}/ChatServer/Headers/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ChatServer/Source/*.cpp
)

file(GLOB SOURCE_FILES_RESTSERV
    ${CMAKE_CURRENT_SOURCE_DIR}/RestServer/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/RestServer/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RestServer/Controllers/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/RestServer/Controllers/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RestServer/Services/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/RestServer/Services/*.cpp
)

add_library(ACA-Database-lib STATIC ${SOURCE_FILES_DATABASE})
target_include_directories(ACA-Database-lib PUBLIC ${INCLUDE_DIR})
target_link_libraries(ACA-Database-lib PRIVATE ${LINK_LIBRARIES})

add_library(ACA-RestServer-lib STATIC ${SOURCE_FILES_RESTSERV})
target_include_directories(ACA-RestServer-lib PUBLIC ${INCLUDE_DIR})
target_link_libraries(ACA-RestServer-lib PRIVATE ${LINK_LIBRARIES})

add_library(ACA-ChatServer-lib STATIC ${SOURCE_FILES_CHATSERV})
target_include_directories(ACA-ChatServer-lib PUBLIC ${INCLUDE_DIR})
target_link_libraries(ACA-ChatServer-lib PRIVATE ${LINK_LIBRARIES})


add_executable(ACA-RestServer mainRestServer.cpp)
target_include_directories(ACA-RestServer PUBLIC ${INCLUDE_DIR})
target_link_libraries(ACA-RestServer PRIVATE ${LINK_LIBRARIES} ACA-Database-lib ACA-RestServer-lib)

add_executable(ACA-ChatServer mainChatServer.cpp)
target_include_directories(ACA-ChatServer PUBLIC ${INCLUDE_DIR})
target_link_libraries(ACA-ChatServer PRIVATE ${LINK_LIBRARIES} ACA-Database-lib ACA-ChatServer-lib)