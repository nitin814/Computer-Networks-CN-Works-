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
    int sfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sfd == -1)
    {
        cout<<"Error in creating socket "<<endl;
        return 0;
    }
    struct sockaddr_in serveraddress;
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_addr.s_addr = INADDR_ANY;
    serveraddress.sin_port = htons(6666);
    
    int binderror = bind (sfd , (struct sockaddr*)&serveraddress , sizeof(serveraddress));
    if (binderror==-1)
    {
        cout<<"Error in binding "<<endl;
        return 0;
    } 
    if (listen (sfd , 10)==-1)
    {
        cout<<"error in listening "<<endl;
        return 0;
    }

    while (1)
    {
        struct sockaddr_in clientaddress;
        socklen_t len = sizeof(clientaddress);

        int nsfd = accept (sfd , (sockaddr *)&clientaddress , &len);

        cout<<"WIll now try to do hacking stufss , i will manipulate ip address for the mac comming ! HAHAHA"<<endl;
    }
}