#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <sys/wait.h>
#include <poll.h>
using namespace std;

int main ()
{
    int sfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sfd==-1)
    {
        cout<<"Error in creating socket"<<endl;
        return 1;
    }
    int port; cout<<"ENter which port you want to join .."<<endl;
    cin>>port;

    struct sockaddr_in serveraddress;
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(port);
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.0");

    int t = connect (sfd , (sockaddr *)&serveraddress , sizeof(serveraddress));
    if (t==-1)
    {
        cout<<"Error in connecting "<<endl;
        return 1;
    }
    cout<<"connected"<<endl;

    while (1)
    {
        int type;
        cout<<"Enter type of message to be sent : "<<endl;
        cin>>type;
        if (type==1)
        {
            int sfdless = socket(AF_INET , SOCK_DGRAM , 0);
            if (sfdless==-1)
            {
                cout<<"Error in creating socket"<<endl;
                return 1;
            }

            struct sockaddr_in serveraddress;
            serveraddress.sin_family = AF_INET;
            serveraddress.sin_port = htons(9999);
            serveraddress.sin_addr.s_addr = inet_addr("127.0.0.0");
            
            cout<<"asking all servers which are present through F process ... "<<endl;
            string str = "1";
            const char * buf = str.c_str();
            int n = sendto(sfdless , buf , strlen(buf)+1 , 0 , (struct sockaddr *)&serveraddress , sizeof(serveraddress));
            
            struct sockaddr_in clientaddress;
            char buffer[100]; 
            socklen_t client_len = sizeof(clientaddress);
            n = recvfrom(sfdless , &buffer , sizeof(buffer)-1 , 0 , (struct sockaddr *)&clientaddress , &client_len);
            buffer[n] = '\0';
            sleep(0.5);
            cout<<"THe port of servers which are present available are : "<<buffer<<endl;
        }
        else if (type==2)
        {
            cout<<"enter text to send to service : "<<endl;
            string str;
            cin>>str;
            const char * buf = str.c_str();
            cout<<"sending the text : "<<str<<endl;
            send(sfd , buf , strlen(buf) , 0);
        }  
    }
}