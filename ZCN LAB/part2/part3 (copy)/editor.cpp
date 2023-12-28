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

void sendSfd(int sfd)
{
    sleep(1);
    int usfd=socket(AF_UNIX, SOCK_DGRAM, 0);
    struct sockaddr_un uAddr;
    uAddr.sun_family = AF_UNIX;
    strcpy(uAddr.sun_path, "socketfile");

    struct iovec e = {NULL, 0};
    char cmsg[CMSG_SPACE(sizeof(int))]; 
    struct msghdr m = {(void*)&uAddr, sizeof(uAddr), &e, 1, cmsg, sizeof(cmsg), 0};
    struct cmsghdr *c = CMSG_FIRSTHDR(&m);
    c->cmsg_level = SOL_SOCKET;
    c->cmsg_type = SCM_RIGHTS;
    c->cmsg_len = CMSG_LEN(sizeof(int));
    *(int*)CMSG_DATA(c) = sfd;
    if(sendmsg(usfd, &m, 0)<0){
        perror("sendmsg err");
        exit(1);
    }
    cout<<"sent sfd"<<endl;
}

int main ()
{
    int fd1 = fileno(popen("./reporter1" , "r"));
    int fd2 = fileno(popen("./reporter2" , "r"));
    int fd3 = fileno(popen("./reporter3" , "r"));

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

                    if (buffer[0]=='d')
                    {   
                        cout<<"Now i will send the report to document ... "<<endl;
                        sleep(2);
                        sendSfd(pfd[i].fd);
                        sleep(2);
                    }
                    else
                    {
                        cout<<"Now i am sending to news"<<endl;
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
}