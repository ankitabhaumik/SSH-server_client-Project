#ifndef TCPSERVER_H
#define TCPSERVER_H

#include<string>
using namespace std;


class TCPServer
{
   public:
   virtual bool initialize(int port) = 0;
   virtual void start()=0;
   virtual ~TCPServer() = default;
};

//Factory function to create the appropriate server instace based on OS

TCPServer* createServer();


#endif