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

struct data 
{
    int low;
    int high;
    int arr[100];
};

int getSfd()
{
    cout<<"My pid : "<<getpid()<<endl;
    int usfd=socket(AF_UNIX, SOCK_DGRAM, 0);
    struct sockaddr_un un;
    un.sun_family = AF_UNIX;
    unlink("socketfile");
    strcpy(un.sun_path, "socketfile");

    if (bind(usfd, (struct sockaddr*)&un, sizeof(un)) < 0) {
        perror("usfd bind err");
        return 1;
    }
    char buf[512];
    struct iovec e = {buf, 512};
    char cmsg[CMSG_SPACE(sizeof(int))];
    struct msghdr m = {NULL, 0, &e, 1, cmsg, sizeof(cmsg), 0};
    if(recvmsg(usfd, &m, 0)<0){
        perror("usfd recvmsg err");
        exit(1);
    }
    struct cmsghdr *c = CMSG_FIRSTHDR(&m);
    int sfd = *(int*)CMSG_DATA(c);
    return sfd;
}

int main ()
{
    while (1)
    {
        int sfd = getSfd();

        char buffer[100];
        int n = read(sfd , buffer , sizeof(buffer)-1);
        buffer[n] = '\0';

        cout<<"I am document store and i read "<<buffer<<endl;

        close(sfd);
    }
}