cmake_minimum_required (VERSION 3.3) 
project (SSH_server_client) #project name

#specify the c++ standard
set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED True) 


#Detect the platforms and define variable accordingly
#if(UNIX AND NOT APPLE)
#   set(PLATFORM_NAME "linux")
#   add_definitions(-DPLATFORM_LINUX)
#endif()

add_subdirectory(tcpserver)
add_subdirectory(tcpclient)

#server example build
add_executable(server server.cpp)
target_link_libraries(server tcpserver)
set(INCLUDE_SERVER_DIRS "../tcpserver")
target_include_directories(server PUBLIC ${INCLUDE_SERVER_DIRS})



#client example build
add_executable(client client.cpp)
target_link_libraries(client tcpclient)
set(INCLUDE_CLIENT_DIRS "../tcpclient")
target_include_directories(client PUBLIC ${INCLUDE_CLIENT_DIRS})