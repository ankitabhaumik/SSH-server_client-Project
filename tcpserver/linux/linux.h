#ifndef LINUXSERVER_H
#define LINUXSERVER_H
#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h> 
#include<iostream>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<unistd.h>
#include<fcntl.h>
#include <thread>
#include <unordered_map>
#include "../ctpl_stl.h"
#include "../tcpserver.h"
using namespace std;

#define MAX_EVENTS 10
#define BUFFER_SIZE 1024

class LinuxServer : public TCPServer
{
    private:
    int server_fd;
    thread epollthread;
    int epoll_fd;
    unordered_map<string,string>mp;
  

    void epollLoop();
    bool setSocketNonBlocking(int socketId);
    void handleRecv(int client_socket);
    static int determineThreadPoolSize(); // Helper to determine the thread pool size
    void handleSend(int client_socket);
    void handleError(int client_socket);
    static ctpl::thread_pool threadPool;  // Static thread pool shared by all instances
    bool handleClient(int client_socket);
    


   public:
   LinuxServer();
   virtual ~LinuxServer();

   bool initialize(int port); //const string& ipAdress = "127.0.0.1");

   void start();


};


#endif