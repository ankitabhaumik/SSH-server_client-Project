#include<iostream>
#include<stdio.h>
#include "tcpserver.h"
using namespace std;


int main()
{
    TCPServer* server = createServer();

    if(server)
    {
       if(!server->initialize(8000))
       {
         cout<<"initialize failed"<<endl;
         return -1;
       }

       server->start();

       delete server;

    }
    else
    {
        cout<<"Failed to create server"<<endl;
    }

    return 0;
}