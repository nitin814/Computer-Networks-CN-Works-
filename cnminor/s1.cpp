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
int sfdget = -1;

void func1 (int signo)
{
    struct sockaddr_in clientaddress;
    socklen_t len = sizeof(clientaddress);

    int nsfd = accept (sfdget , (sockaddr *)&clientaddress , &len);
    cout<<"Got a client "<<endl;
    send(nsfd , "Hello" , 5 , 0);
    cout<<"Doing work "<<endl;
}

void func2 (int signo)
{
    cout<<"Blocked ip incomming "<<endl;
    struct sockaddr_in clientaddress;
    socklen_t len = sizeof(clientaddress);

    int nsfd = accept (sfdget , (sockaddr *)&clientaddress , &len);
    close(nsfd);
}

int main ()
{
    signal (SIGUSR1 , func1);
    signal (SIGUSR2 , func2);
    int sfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sfd == -1)
    {
        cout<<"Error in creating socket "<<endl;
        return 0;
    }
    cout<<"Enter sg pid value : "<<endl;
    int sg; cin>>sg;
    cout<<"Killing it , killed"<<endl;
    struct sockaddr_in serveraddress;
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_addr.s_addr = INADDR_ANY;
    serveraddress.sin_port = htons(8889);
    
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

    struct pollfd fds[1];
    fds[0].fd = sfd;
    fds[0].events = POLLIN;

    while (1)
    {
        int s = poll (fds , 1 , 1000);
        if (s>0)
        {
            for (int i =0;i<1;i++)
            {
                if (fds[i].revents & POLLIN)
                {   
                    sfdget = fds[i].fd;
                    sleep(2);
                    kill(sg , SIGUSR1);
                    sleep(2);
                }
            } 
        }
    }   
}