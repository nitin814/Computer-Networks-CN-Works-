#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/un.h>
#include <poll.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <bits/stdc++.h>
using namespace std;

int main ()
{
    // connection less connection making ...
    int usfd = socket(AF_INET , SOCK_DGRAM , 0);
    if (usfd==-1)
    {
        cout<<"Error in creating socket"<<endl;
        return 1;
    }
   
    struct sockaddr_in serveraddress,clientaddress;
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(9999);
    serveraddress.sin_addr.s_addr = INADDR_ANY;

    int binderror = bind (usfd , (struct sockaddr *)&serveraddress , sizeof(serveraddress));
    if (binderror == -1)
    {
        cout<<"Error in binding "<<endl;
        return 0;
    }

    cout<<"Unix done succesfully"<<endl;
    // connection oriented connection making ...
    int sfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sfd == -1)
    {
        cout<<"Error in creating socket "<<endl;
        return 0;
    }

    struct sockaddr_in serveraddres;
    serveraddres.sin_family = AF_INET;
    serveraddres.sin_port = htons(8889);
    serveraddres.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    int t = connect (sfd , (sockaddr *)&serveraddres , sizeof(serveraddres));
    if (t==-1)
    {
        cout<<"Error in connecting "<<endl;
        return 1;
    }

    sleep(6);

    // getting a usfd message 
    char buffer[100]; 
    socklen_t client_len = sizeof(clientaddress);
    recvfrom(usfd , &buffer , sizeof(buffer) , MSG_DONTWAIT , (struct sockaddr *)&clientaddress , &client_len);
    if (strlen(buffer) > 0)
    {
        cout<<buffer<<endl;
        cout<<"yoyoy"<<endl;
        cout<<"BHAIYA DOING HACKER STUFFS !! OP !!"<<endl;

        int sfd2 = socket(AF_INET , SOCK_STREAM , 0);
        if (sfd2 == -1)
        {
            cout<<"Error in creating socket "<<endl;
            return 0;
        }

        struct sockaddr_in serveraddres;
        serveraddres.sin_family = AF_INET;
        serveraddres.sin_port = htons(6666);
        serveraddres.sin_addr.s_addr = inet_addr("127.0.0.1");
        
        int t = connect (sfd2 , (sockaddr *)&serveraddres , sizeof(serveraddres));
        if (t==-1)
        {
            cout<<"Error in connecting "<<endl;
            return 1;
        }
        cout<<"Proceeding to hacking"<<endl;
    }
    else
    {
        char buffer[1000]; int n;
        n = recv(sfd , buffer , sizeof(buffer)-1 , 0);
        buffer[n] = '\0';
        cout<<buffer<<endl;
    }
}