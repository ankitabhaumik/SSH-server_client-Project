# Project Server

project(tcpserver)

#if(UNIX AND NOT APPLE)
set(SERVER_OS_SRC linux/linux.cpp)
#endif()

add_library(tcpserver STATIC tcpserver.cpp ${SERVER_OS_SRC})

target_include_directories(tcpserver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})