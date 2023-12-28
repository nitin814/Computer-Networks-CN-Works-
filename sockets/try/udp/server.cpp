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
    int sfd = socket(AF_INET , SOCK_DGRAM , 0);
    if (sfd==-1)
    {
        cout<<"Error in creating socket"<<endl;
        return 1;
    }
    
    struct sockaddr_in serveraddress,clientaddress;
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(55554);
    serveraddress.sin_addr.s_addr = INADDR_ANY;

    int binderror = bind (sfd , (struct sockaddr *)&serveraddress , sizeof(serveraddress));
    if (binderror == -1)
    {
        cout<<"Error in binding "<<endl;
        return 0;
    }

    while (1)
    {
        char buffer[100]; 
        socklen_t client_len = sizeof(clientaddress);
        recvfrom(sfd , &buffer , sizeof(buffer) , MSG_DONTWAIT , (struct sockaddr *)&clientaddress , &client_len);
        cout<<buffer<<endl;
        cout<<"yoyoy"<<endl;
    }
}