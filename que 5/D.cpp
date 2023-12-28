#include <bits/stdc++.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/select.h>
#include<pthread.h>
#include<signal.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/un.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<pcap.h>
#include<errno.h>
#include<netinet/if_ether.h>
#include<net/ethernet.h>
#include<netinet/ether.h>
#include<netinet/udp.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include <poll.h>
#include <sys/types.h>

using namespace std;

int x,y,z;
int cnt = 0;
int pids[2];
bool flag = true;

vector<int> storefds;
// for sending File Descriptor 
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

// stop the process ...
void func (int signo)
{
    cout<<"Sorry for corruption trouble , Bye Bye ..."<<endl;
    flag = false;
    return ;
}

// send message to existing clients that petrol/diesel/gas is not available
void * notify (void * args)
{
    if (x==0)
        for (auto i : storefds)
            send(i , "petrol done" , 11 , 0);
    if (y==0)
        for (auto i : storefds)
            send(i , "diesel done" , 11 , 0);
    if (z==0)
        for (auto i : storefds)
            send(i , "gas done" , 8 , 0);

    return NULL;
}

int main ()
{
    cout<<"I am D for SC , and my pid is : "<<getpid()<<endl;
    cout<<"Enter x,y,x"<<endl;
    cin>>x>>y>>z;

    // we can use shared memory for it , but I am using this for simplicity
    cout<<"Enter pids of B1 and B2"<<endl;
    cin>>pids[0]>>pids[1];

    
    int sfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sfd==-1)
    {
        cout<<"Error in creating socket"<<endl;
        return 1;
    }
    
    signal(SIGUSR1 , func);

    struct sockaddr_in serveraddress;
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(9998);
    serveraddress.sin_addr.s_addr = INADDR_ANY;

    int binderror = bind (sfd , (struct sockaddr *)&serveraddress , sizeof(serveraddress));
    if (binderror == -1)
    {
        cout<<"Error in binding "<<endl;
        return 0;
    }
  
    int listenerror = listen(sfd , 10);
    if (listenerror == -1)
    {
        cout<<"Listening error is found "<<endl;
        return 0;
    }

    while (1)
    {
        if (flag==false)
            break;
        struct sockaddr_in clientaddr;
        socklen_t len = sizeof(clientaddr);

        // accept the connection request from V
        int newfd = accept(sfd , (struct sockaddr*)&clientaddr , &len);
        if (newfd == -1)
        {
            cout<<"not"<<endl;
            continue;
        }

        // store them , so that we can send messgae when either petrol/disel/gas is not available
        storefds.push_back(newfd);
        char buffer[100];
        // getting knowlege of petrol/disel/gas from V
        int n = recv(newfd , buffer , sizeof(buffer)-1 , 0);
        buffer[n] = '\0';

        if (strcmp(buffer , "petrol")==0)
            x--;
        else if (strcmp(buffer , "diesel")==0)
            y--;
        else
            z--;
        
        // sending it alternatively
        if (cnt%2==0)
            kill(pids[0] , SIGUSR1);
        else
            kill(pids[1] , SIGUSR1);
        
        sleep(1);
        sendSfd(newfd);

        // thread to avoid latency / blocking of accpet calls 
        pthread_t notifys;
        pthread_create(&notifys , NULL , notify , NULL);
        
        cnt++;
    }
}