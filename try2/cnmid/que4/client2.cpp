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
int shmid1;

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

void handler (int signo)
{
    int t = 5; int fdpass = -1;

    fdpass = getSfd();
    
    while (t--)
    {
        cout<<"Enter a string , i am pid "<<getpid()<<" : "<<endl;
        string str;
        cin>>str;
        send(fdpass , str.c_str() , str.size() , 0);
    }

    struct data * res1 = (struct data *)shmat(shmid1 , 0 , 0);

    res1->arr[res1->high+1] = getpid();
    res1->high++;
    kill(res1->arr[res1->low+1] , SIGUSR1);
    res1->low++;
    
    sleep(1);
    
    cout<<"sending message "<<endl;
    
    sendSfd(fdpass);
}

int main ()
{
    signal(SIGUSR1 , handler);

    key_t key1 = ftok("./input.txt" , 'A');
    if (key1==-1)
    {
        cout<<"Key is not valid one . "<<endl;
        return 0;
    }
    shmid1 = shmget(key1 , 512 , IPC_CREAT | 0666);
    if (shmid1 == -1)
    {
        cout<<"couldnt make a SM "<<endl;
        return 0;
    }
    
    struct data * res1 = (struct data *)shmat(shmid1 , 0 , 0);
    res1->arr[res1->high+1] = getpid();
    res1->high++;

    while (1)
    {

    }
}