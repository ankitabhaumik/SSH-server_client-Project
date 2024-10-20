#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <stdio.h>
#include <string>



using namespace std;


class TCPClient
{

   public:
   virtual bool initialize() = 0;
   virtual bool start(unsigned int port,string &ipAdress) = 0;
   virtual string receiveData() = 0;
   virtual bool sendData(string &data) = 0;
   virtual ~TCPClient() = default;

};


TCPClient* createclient();



#endif