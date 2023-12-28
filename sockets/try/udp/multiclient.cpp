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

    struct sockaddr_in serveraddress;
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(55554);
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (1)
    {
        string str;
        getline(cin , str);
        const char * buffer = str.c_str();
        sendto(sfd , buffer , strlen(buffer)+1 , 0 , (struct sockaddr *)&serveraddress , sizeof(serveraddress));
    }
}