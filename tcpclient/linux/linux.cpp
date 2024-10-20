#include "linux.h"


LinuxClient::LinuxClient(){
   client_socket = -1;
}

LinuxClient::~LinuxClient()
{
    if (client_socket != -1)
    {
        close(client_socket);
    }
}


bool LinuxClient::initialize()
{
    // No explicit initialization required on Linux
    return true;
}

bool LinuxClient::sendData(string &data)
{
    cout<<"username 2 :"<<data<<endl;
    ssize_t result = send(client_socket,data.c_str(),strlen(data.c_str()),0);
    if (result<=0)
    {
        cout<<"Error in sending data"<<endl; //ssize_t result = 
        return false;
    }

    cout<<"Buffer Send "<<result<<endl;
    return true;
}


string LinuxClient::receiveData()
{
    char buffer[BUFFER_SIZE];
    memset(buffer,0,sizeof(buffer));
    ssize_t result = recv(client_socket,buffer,BUFFER_SIZE,0);
    if(result>0)
    {
        string data(buffer,result);
        return data;
    }
    else if(result == 0)
    {
        cout<<"Connection closed by the server"<<endl;
        close(client_socket);
    }
    else
    {
        cout<<"Receive failed : "<<strerror(errno)<<endl;
    }

    return "";

}

bool LinuxClient::start(unsigned int port, string& ipAdress)
{
    //Creation of socket
     client_socket = socket(AF_INET, SOCK_STREAM,0); //Socket descriptor
     if(client_socket < INVALID_SOCKET)
     {
        cout<<"FAiled to create the socket\n";
        return false;
     }

    //Set the server address structure
     struct sockaddr_in client_addr;
     memset(&client_addr,0,sizeof(client_addr));
     client_addr.sin_family = AF_INET;
     client_addr.sin_port = htons(port); //convert port to netowk byte order

    //convert ip address from text to binary form
     if(inet_pton(AF_INET,ipAdress.c_str(),&client_addr.sin_addr) <= 0)
     {
        cout<<"Invalid address or address not supported\n";
        close(client_socket);
        return false; //Exit if address conversion fails
     }

     //Connect the socket
     if(connect(client_socket,(struct sockaddr*) &client_addr, sizeof(client_addr)) < 0)
     {
        cout<<"Failed to bind the socket\n";
        close(client_socket);
        return false;
     }

     cout<<"Successfully connected to the server with ip "<<ipAdress<<" port "<<port<<endl;
     return true;

}

