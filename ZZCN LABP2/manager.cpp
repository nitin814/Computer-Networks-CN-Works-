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

int rsfd = 0;
int sfd = 0;
int advtid = 0;
// make sure to put in vector so that we can poll and send to clients ...
int newfd = 0;

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
// getting data from live process , dont accept advertisement one ....
void * thread1 (void * args)
{
    struct sockaddr_in client;
	client.sin_family=AF_INET;
	client.sin_addr.s_addr=inet_addr("127.0.0.1");

    while (1)
    {
        char buffer[100];
        int n = recv(newfd , buffer , sizeof(buffer)-1 , 0);
        kill (advtid , SIGUSR1);
        buffer[n] = '\0';
 
        cout<<"received "<<buffer<<endl;
        const char * buff = buffer;
        unsigned int client_len=sizeof(client);
        cout<<"sending to my channels "<<endl;
        sendto(rsfd,buff,strlen(buff)+1,0,(struct sockaddr*)&client,sizeof(client));
        cout<<"sended "<<endl;
        sleep(1);
        kill (advtid , SIGUSR2);
    }
}
// accepting the live process ..
void * thread2 (void * args)
{
    struct sockaddr_in clientaddr;
    socklen_t len = sizeof(clientaddr);
    newfd = accept(sfd , (struct sockaddr*)&clientaddr , &len);
    if (newfd == -1)
    {
        cout<<"not"<<endl;
    }
    return NULL;
}
// sending sfd
void sending (int sig)
{
    cout<<"sending fd"<<endl;
    sleep(0.2);
    sendSfd(rsfd);
    cout<<"sent fd"<<endl;
}

int main ()
{
    cout<<"ENter the pid of advertisement "<<endl;
    cin>>advtid;
    key_t key1 = ftok("./input.txt" , 'A');
    if (key1==-1)
    {
        cout<<"Key is not valid one . "<<endl;
        return 0;
    }
    int shmid1 = shmget(key1 , 512 , IPC_CREAT | 0666);
    if (shmid1 == -1)
    {
        cout<<"couldnt make a SM "<<endl;
        return 0;
    }
    // using shared memory for getting the pid of advertisement process ...
    int * res1 = (int *)shmat(shmid1 , 0 , 0);
    *res1 = getpid();

    cout<<"my pid is "<<getpid()<<endl;
    signal(SIGUSR1 , sending);
    rsfd = socket(AF_INET,SOCK_RAW,20);
    if(rsfd<0)
    {
        cout<<"Socket Error\n";
        return 1;
    }


    sfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sfd==-1)
    {
        cout<<"Error in creating socket"<<endl;
        return 1;
    }

    struct sockaddr_in serveraddress;
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(3000
    
    );
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

    pthread_t t1; pthread_t t2;
    
    // accepting live servers 
    pthread_create(&t2 , NULL , thread2 , NULL);
    pthread_join(t2 , NULL);

    // getting the data and then sending it back to news channels 
    pthread_create(&t1 , NULL , thread1 , NULL);
    pthread_join(t1 , NULL); 
    

    return 0;
}