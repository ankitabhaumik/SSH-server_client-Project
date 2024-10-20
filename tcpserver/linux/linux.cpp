#include "linux.h"

LinuxServer::LinuxServer() 
{
   server_fd = -1;
   epoll_fd =-1;
   mp["username"] = "password";
   mp["linux"] = "linuxpass";

}

LinuxServer::~LinuxServer()
{
    if (server_fd != -1)
    {
        close(server_fd);
    }
    if (epollthread.joinable())
    {
        epollthread.join();
    }

}

ctpl::thread_pool LinuxServer::threadPool(LinuxServer::determineThreadPoolSize());

int LinuxServer::determineThreadPoolSize()
{
   int concurrency = thread::hardware_concurrency();
   return (concurrency>0)? concurrency : 4;
}


//to make any socket non blocking
bool LinuxServer :: setSocketNonBlocking(int socketId)
{ 
   int flags = fcntl(socketId, F_GETFL,0); //First get the flags whatever is set for that socketid using this fcntl
   if(flags == -1)  
   {
      cout<<"Error getting Socket flags.\n";
      return false;
   }

   flags |= O_NONBLOCK; //Now after getting the flag we will do or operation with non blocking flag 
   if(fcntl(socketId, F_SETFL, flags) == -1) //and then set the same flag in the same socketid
   {
      cout<<"Error setting socket to non blocking" <<endl;
      return false;
   }

   cout<<"Socket set to non blocking mode."<<endl;
   return true;

}



bool LinuxServer::initialize(int port)  // , const string& ipAdress)
{
    int opt =1;
    //Creation of socket
      if((server_fd = socket(AF_INET, SOCK_STREAM,0)) == 0) //Socket descriptor
      {
        cout<<"FAiled to create the socket"<< strerror(errno) << endl;
        return false;
      }

     //Set socket options to reuse address and port
     //for this server_fd we need to set socket option to SO_REUSEADDR and SO_REUSEPORT, and option we are setting it as 1
     //if for any situatuon port got killed the after restart there can be chance that the same port may not be usable for connection
     //so to avoid that we use setsocketopt so that my port can free quickly and my server can restart with same port
      if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, &opt, sizeof(opt)))
      {
        cout<<"Setsocket failed"<< strerror(errno) << endl;
        return false;
      }

      //Epoll creation
      //----------------
      epoll_fd = epoll_create1(0); //0 use the older epoll 
      if(epoll_fd == -1)
      {
        cout<<"FAiled to create EPoll"<<endl;
        return false;
      }
 
    
    //Set the server address structure
     struct sockaddr_in server_addr;
     server_addr.sin_family = AF_INET;
     server_addr.sin_port = htons(port); //convert port to netowk byte order
     server_addr.sin_addr.s_addr = INADDR_ANY; //accept connection from any server

     //Bind the socket
     if(bind(server_fd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0)
     {
        cout<<"Failed to bind the socket"<< strerror(errno) << endl;
        close(server_fd);
        return false;
     }

     //Listen to the socket
     if(listen(server_fd,SOMAXCONN) < 0)
     {
        cout<<"Failed to listen to the socket.\n"<< strerror(errno) << endl;
        close(server_fd);
        return false;
     }

     cout<<"Server initialized on port :" << port<<endl;
     return true;

}

void LinuxServer::epollLoop()
{
   struct epoll_event events[MAX_EVENTS];

   while(true)
   {
      int event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1); //blocking call

      for(int i=0;i<event_count;i++)
      {
         int fd = events[i].data.fd;
         if(events[i].events & EPOLLIN)
         {
            //Push to the threadpool to receive the data
            threadPool.push([this,fd] (int threadid)
                            {return this->handleRecv(fd);}); // if it is return, that means there was no data,  
                                                            //again it will go to recv if any data comes and go to handlerecv 
                                                            // OR go to next event
         }
         /*else if(events[i].events & EPOLLOUT)
         {
            //Push to the threadpool to send the data
            threadPool.push([this,fd] (int threadid)
                            {return this->handleSend(fd);});
         }
         else if(events[i].events & EPOLLERR)
         {
            //Push to the threadpool to Handle the Error
            threadPool.push([this,fd] (int threadid)
                            {return this->handleError(fd);});
         }*/


      }
   }


}

#if 0
string executecommand(int client_socket, string &command)
{
   string data;
   
     // Execute the command and send the output back to the client
        FILE *pipe = popen(command.c_str(), "r");
        if (!pipe) {
            data = "Error executing command\n";
            return data;
        } else {
            while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
                send(client_socket, buffer, sizeof(buffer), 0);
            }
            send(client_socket, "<end>", 5, 0);  // Signal end of command output
            usleep(5000);  // Short delay to ensure the client receives all data
            pclose(pipe);
        }
    }
    close(client_socket);  // Close the client socket

}

#endif

bool LinuxServer :: handleClient(int client_socket)
{
    char buffer[1024] = {0};
    string username,password;

    //Username
    string str = "Enter Username : ";
    send(client_socket,str.c_str(),strlen(str.c_str()),0);


    ssize_t recv_bytes = recv(client_socket,buffer,strlen(buffer),0);
    cout<<"username in server : "<<buffer<<endl;
    if(recv_bytes>0)
    {
      buffer[recv_bytes] = '\0';
       username = buffer;
       cout<<"username : "<<username<<endl;
    }
    else
    {
       cout<<"username did not came properly : "<<recv_bytes<<endl;
       //close(client_socket);
        return false;
    }


    //Password
    str = "Enter Password : ";
    send(client_socket,str.c_str(),strlen(str.c_str()),0);
  

    recv_bytes = recv(client_socket,buffer,strlen(buffer),0);
    if(recv_bytes>0)
    {
      buffer[recv_bytes] = '\0';
       password = buffer;
       cout<<"password : "<<password<<endl;
    }
    else
    {
       cout<<"password did not came properly"<<endl;
       close(client_socket);
        return false;
    }

    if(mp.find(username) != mp.end())
    {
       if(mp[username] == password)
       {
          str = "Authetication successfull";
          send(client_socket,str.c_str(),strlen(str.c_str()),0);
         //  if(send_bytes<0)
         //  {
         //       cout<<"Send Failed";send_bytes = 
         //       return false;
         //  }
       }
    }
    else
    {
         cout<<"Authetication failed"<<endl;
         // int send_bytes = send(client_socket,"Authetication failed\n",1024,0);
         //  if(send_bytes<0)
         //  {
         //       cout<<"Send Failed";
         //       return false;
         //  }
    }

    epollthread = thread(&LinuxServer::epollLoop,this);

    //Set the client socket as non blocking
    setSocketNonBlocking(client_socket);

    //Add the new client socket to the epoll set for monitoring.
   struct epoll_event ev;
   ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
   ev.data.fd = client_socket; 
   epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_socket, &ev);

  return true;

}

void LinuxServer :: handleRecv(int client_socket)
{
   //Receive msg from client
     char buffer[BUFFER_SIZE];
     string command;
     int bytes_received = recv(client_socket,buffer,sizeof(buffer),0);

     if(bytes_received > 0)
     {
          buffer[bytes_received] ='\0';
          printf("Message from client :  %s\n", buffer);
          command = buffer;
          if(command == "exit")
          {
            close(client_socket);
            return;
          }
     }
     else if(bytes_received == 0)
     {
       //socket is close if client close the connection it will return zero
       close(client_socket);
     }
     else
     {
        // it is for each thread
        if(errno != EWOULDBLOCK && errno != EAGAIN) //Error has occured, if it is something else then error occured                                             
        {
            printf("Received failed. Bytes received : %d\n", bytes_received);
            close(client_socket); //closed the socket;
        }
        else
        {
             //if the bytes recv is -1 meanns nothing is there in the buffer but the errorno is EWOULDBLOCK and EAGAIN
            //that means there is nothing wrong in the socket , this time data is not recv may be it will recv later.
             return;
        }
     }

   
   //string res = executecommand(client_socket,command);
   //handleSend(res);


}




void LinuxServer::start()
{
    //epollthread = thread(&LinuxServer::epollLoop,this);
    struct sockaddr_in client_address;
    socklen_t addrlen = sizeof(client_address);
    int client_socket =-1;

    while (true)
    {
        // Accept the connection of the clint continuosly , infinite loop
        //Accept incoming connection

        if ((client_socket = accept(server_fd, (struct sockaddr *)&client_address,&addrlen)) < 0)
        {
            cout << "Accept failed! Error: " << strerror(errno) << endl;
            //continue;
        }


     cout<<"Connection accept has successfully done"<<endl;
     handleClient(client_socket);
    

    }
}