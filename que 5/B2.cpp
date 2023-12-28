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

pthread_mutex_t locks; 
int shmid1;
struct message
{
    char ips[50];
};

struct data
{
    int lastindex;
    int lastindexread;
    struct message messagee[30];
};

struct data * res1;

// getting the fd 
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


void * takeandprocess (void * args)
{
    int nsfd = getSfd();
    cout<<"got sfd"<<endl;
    char buf[100];
    recv(nsfd , &buf , 100 , 0);
    cout<<"Amount  : "<<buf<<endl;

    struct sockaddr_storage peer_addr;
    socklen_t addr_len = sizeof(peer_addr);
    // extracting the ip of V and storing it in shared memory 
    if (getpeername(nsfd, (struct sockaddr*)&peer_addr, &addr_len) == 0) 
    {
        string ip_str;
        if (peer_addr.ss_family == AF_INET) 
        {
            struct sockaddr_in *ipv4 = (struct sockaddr_in*)&peer_addr;
            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(ipv4->sin_addr), ip, INET_ADDRSTRLEN);
            ip_str = ip;
        } 
        int random = rand()%5;
        if (random!=0)
        {
            pthread_mutex_lock(&locks); 
            cout<<"got ip of "<<ip_str<<endl;
            int in = ++(res1->lastindex);
            strcpy(res1->messagee[in].ips , ip_str.c_str());
            pthread_mutex_unlock(&locks); 
        }
    } 
    else 
    {
        cout<<"noooo"<<endl;
        perror("getpeername");
    }
    return NULL;
}
// when get a signal from V , create a thread and get the fd and store the vehicle id (ip) in shared memory
void func(int sig)
{
    pthread_t takefd;
    pthread_create(&takefd , NULL , takeandprocess , NULL);
}

int main ()
{
    key_t key1 = ftok("./input2.txt" , 'A');
    if (key1==-1)
    {
        cout<<"Key is not valid one . "<<endl;
        return 0;
    }
    
    cout<<"My pid is : "<<getpid()<<endl;

    signal(SIGUSR1 , func);

    shmid1 = shmget(key1 , 512 , IPC_CREAT | 0666);
    if (shmid1 == -1)
    {
        cout<<"couldnt make a SM "<<endl;
        return 0;
    }

    res1 = (struct data *)shmat(shmid1 , 0 , 0);

    res1->lastindex = 0; res1->lastindexread = 0;

    // mutex initialization
    if (pthread_mutex_init(&locks, NULL) != 0) 
    { 
        printf("\n mutex init has failed\n"); 
        return 1; 
    } 

    while (1)
    {

    }

    int check = shmdt((void *)res1);

    if (check==-1)
    {
        cout<<"Detaching did not happen successfully "<<endl;
        return 0;
    }
}