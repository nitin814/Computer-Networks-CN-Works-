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

struct messagetoken
{
    int id;
};

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
    int fd1 = fileno(popen("./reporter" , "r"));
    int fd2 = fileno(popen("./reporter" , "r"));
    int fd3 = fileno(popen("./reporter" , "r"));

    struct pollfd pfd[3];
    pfd[0].fd = fd1; pfd[0].events = POLLIN;
    pfd[1].fd = fd2; pfd[1].events = POLLIN;
    pfd[2].fd = fd3; pfd[2].events = POLLIN;

    int rsfd = socket(AF_INET,SOCK_RAW,10);
    if(rsfd<0)
    {
        perror("socket");
        cout<<"Socket Error\n";
        return 1;
    }

    struct sockaddr_in client;
    client.sin_family=AF_INET;
    client.sin_addr.s_addr=inet_addr("127.0.0.1");

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
                    char buffer[200];
                    int n = read(pfd[i].fd , buffer , 1);
                    buffer[n] = '\0';

                    cout<<"I am editor and i read one "<<buffer<<endl;

                    if (buffer[0]=='d')
                    {   
                        cout<<"Now i will send the report to document ... "<<endl;
                        sleep(2);
                        sendSfd(pfd[i].fd);
                        sleep(2);
                    }
                    else
                    {
                        // cout<<"Now i am sending to news"<<endl;
                        char buf[200]; buf[0]=buffer[0];
                        int n = read(pfd[i].fd , buffer , 200);
                        buffer[n] = '\0';
                        
                        for (int i=0;i<=n;i++)
                        {
                            buf[i+1] = buffer[i];
                        }

                        cout<<"I am editor and i read "<<buf<<endl;
                        struct messagetoken header;
                        // either news1 or news2 will recieve the message ..
                        header.id = (rand()%2+1);

                        char packet[sizeof(buf)+1+sizeof(struct messagetoken)];

                        memcpy(packet, &header, sizeof(messagetoken));
                        memcpy(packet + sizeof(struct messagetoken), buf, sizeof(buf));

                        sendto(rsfd,packet,sizeof(packet),0,(struct sockaddr*)&client,sizeof(client));
                        cout<<"sended"<<endl;
                    }
                }
            } 
        }
    }
}