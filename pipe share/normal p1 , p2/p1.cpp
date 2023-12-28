#include<bits/stdc++.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include<arpa/inet.h>
#include<sys/un.h>
#include<csignal>
using namespace std;

int main ()
{
    int sfd = socket(AF_INET , SOCK_STREAM , 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(3000);

    int binderror = bind(sfd , (struct sockaddr*)&addr , sizeof(addr));
    listen(sfd , 3);

    int usfd = socket(AF_UNIX , SOCK_STREAM , 0);
    struct sockaddr_un addrr;
    addrr.sun_family = AF_UNIX;
    strcpy(addrr.sun_path , "/tmp/socketfile");

    int connectt = connect(usfd , (struct sockaddr*)&addrr , sizeof(addrr));
    if(connectt == -1)
    {
        perror("connect error");
        exit(1);
    }

    int nsfd = accept(sfd , NULL , NULL);
    cout<<"client connected"<<endl;

    cout<<"waiting for p2"<<endl;
   
    char cmsg[CMSG_SPACE(sizeof(int))];
    struct iovec e = {NULL , 0};
    struct msghdr msg = {NULL , NULL , &e , 1 , cmsg , sizeof(cmsg) , 0};
    struct cmsghdr * cmsgptr = CMSG_FIRSTHDR(&msg);
    cmsgptr->cmsg_level = SOL_SOCKET;
    cmsgptr->cmsg_type = SCM_RIGHTS;
    cmsgptr->cmsg_len = CMSG_LEN(sizeof(int));
    *(int *)CMSG_DATA(cmsgptr) = nsfd;
    int n;
    if((n = sendmsg(usfd, &msg, 0))<0)
    {
        perror("sendmsg err");
        exit(1);
    }
    cout<<"naman"<<n<<endl;

    while(1)
    {
        char buffer[1024];
        recv(usfd , buffer , sizeof(buffer),0);
        cout<<"received from p2 : "<<buffer<<endl;
    }
}