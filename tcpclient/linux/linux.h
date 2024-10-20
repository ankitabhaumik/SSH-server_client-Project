#ifndef LINUXCLIENT_H
#define LINUXCLIENT_H
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<arpa/inet.h> 
#include<sys/socket.h>
#include<sys/epoll.h>
#include<unistd.h>
#include<fcntl.h>
#include <thread>
#include "../tcpclient.h"
using namespace std;

#define INVALID_SOCKET 0
#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"
#define PORT 8080

class LinuxClient : public TCPClient
{

   private:
   int client_socket;

   public:
   LinuxClient();
   virtual ~LinuxClient();

   bool initialize();
   bool start(unsigned int port,string& ipAdress);
   string receiveData();
   bool sendData(string &data);


};


#endif




