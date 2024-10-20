# Project client

project(tcpclient)

#if(UNIX AND NOT APPLE)
set(SERVER_OS_SRC linux/linux.cpp)
#endif()

add_library(tcpclient STATIC tcpclient.cpp ${SERVER_OS_SRC})

target_include_directories(tcpclient PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})