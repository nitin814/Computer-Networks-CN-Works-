#include <bits/stdc++.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <csignal>
#include <arpa/inet.h>
#include <poll.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <sys/wait.h>
#include <sys/shm.h>
using namespace std;

int main ()
{
    int fd1 = fileno(popen("./reporter1" , "r"));
    int fd2 = fileno(popen("./reporter2" , "r"));
    int fd3 = fileno(popen("./reporter1" , "r"));

    struct pollfd pfd[3];
    pfd[0].fd = fd1; pfd[0].events = POLLIN;
    pfd[1].fd = fd2; pfd[1].events = POLLIN;
    pfd[2].fd = fd3; pfd[2].events = POLLIN;

    while (1)
    {
        int s = poll(pfd , 3 , 1000);

        if (s>0)
        {
            for (int i=0;i<3;i++)
            {
                if (pfd[i].revents & POLLIN)
                {   
                    cout<<"got message "<<endl;
                    char buffer[100];
                    int n = read(pfd[i].fd , buffer , sizeof(buffer)-1);
                    buffer[n] = '\0';

                    cout<<"I am editor and i read "<<buffer<<endl;
                    cout<<"Now i am sending "<<endl;

                    int sfd = socket(AF_INET , SOCK_STREAM , 0);
                    if (sfd==-1)
                    {
                        cout<<"Error in creating socket"<<endl;
                        return 1;
                    }

                    struct sockaddr_in serveraddress;
                    serveraddress.sin_family = AF_INET;
                    serveraddress.sin_port = htons(3000);
                    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.0");

                    int t = connect (sfd , (sockaddr *)&serveraddress , sizeof(serveraddress));
                    if (t==-1)
                    {
                        cout<<"Error in connecting "<<endl;
                        return 1;
                    }

                    send(sfd , buffer , sizeof(buffer) , 0);

                    cout<<"sended"<<endl;
                }
            } 
        }
    }
}