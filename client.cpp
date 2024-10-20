#include <iostream>
#include <string>
#include <cstring>
#include "tcpclient.h"
using namespace std;

int main()
{
    TCPClient *client = createclient();
    string username, password,command;

    if (client)
    {

        if (!client->initialize())
        {
            cout << "inittialize of client failed" << endl;
            return false;
        }

        string serverip = "0.0.0.0";

        if (!client->start(8000, serverip))
        {
            cout << "start of client failed" << endl;
            return false;
        }

        while (true)
        {

            string str = client->receiveData();

            if(strstr(str.c_str(),"Enter Username : "))
            {
               cout<<str;
               getline(cin,username);
               cout<<"username 1 :"<<username<<endl;
               bool res = client->sendData(username);
               if(res == false)
               {
                cout<<"Username did not sent properly from client"<<endl;
               }

            }
            else if(strstr(str.c_str(),"Enter Password : "))
            {
              cout<<str;
              getline(cin,password);
              client->sendData(password);
            }
            else if(strstr(str.c_str(),"Authetication successfull"))
            {
                 cout<<str;
                 break;
            }
        }


    while(true)
    {
        cout<<"Enter your command , press q to exit : ";
        getline(cin,command);

        client->sendData(command);

        if(command == "q")
        {
            break;
        }

        string str = client->receiveData();
        cout<<"Output is : "<<str<<endl;

    }

     
    cout<<"Connection Closed"<<endl;

    }
}