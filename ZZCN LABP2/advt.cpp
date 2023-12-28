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

int pid;
bool getaccess = true;

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



void * thread1 (void * args)
{
    while (1)
    {
        while (!getaccess);
        cout<<"i came here now "<<endl;
        string str;
        cout<<"Enter something "<<endl;
        getline(cin , str);
        cout<<"got "<<str<<endl;
        cout<<"sending signal to "<<pid<<endl;
        kill(pid , SIGUSR1);
        int rsfd = getSfd();
        struct sockaddr_in client;
        client.sin_family=AF_INET;
        client.sin_addr.s_addr=inet_addr("127.0.0.1");

        const char * buff = str.c_str();
        unsigned int client_len=sizeof(client);
        cout<<"sending to my channels "<<endl;
        sendto(rsfd,buff,strlen(buff)+1,0,(struct sockaddr*)&client,sizeof(client));
        cout<<"sended "<<endl;
        close(rsfd);
    }
}

void block (int signo)
{
    getaccess = false;
    cout<<1<<endl;

    while (getaccess==false)
    {};

    cout<<"now ubnblocked"<<endl;
}

void unblock (int signo)
{
    cout<<"NOw i can take control "<<endl;
    getaccess = true;
    cout<<2<<endl;
}

int main ()
{
    cout<<"my pid is "<<getpid()<<endl;
    int s;
    cin>>s;
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

    int * res1 = (int *)shmat(shmid1 , 0 , 0);
    pid = *res1;

    signal(SIGUSR1 , block);
    signal(SIGUSR2 , unblock);

    pthread_t enters;
    pthread_create(&enters , NULL , thread1 , NULL);
    pthread_join(enters , NULL);
}