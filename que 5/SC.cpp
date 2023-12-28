#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/un.h>
#include <poll.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <bits/stdc++.h>
#define BUF_LEN 65000

using namespace std;

set<string> ips;
int pidkill = 0;

// struct for shared memory
struct message
{
    char ips[100];
};

struct data
{
    int lastindex;
    int lastindexread;
    struct message messagee[100];
};


int main() 
{
    cout<<"Enter pid of D "<<endl;
    cin>>pidkill;
    // capture the TCP packets 
    int rsfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if(rsfd==-1)
    {
        cout<<"ypypy"<<endl;
        return 0;
    }

    key_t key1 = ftok("./input1.txt" , 'A');
    key_t key2 = ftok("./input2.txt" , 'A');

    if (key1==-1 || key2==-1)
    {
        cout<<"Key is not valid one . "<<endl;
        return 0;
    }

    int shmid1 = shmget(key1 , 512 , IPC_CREAT | 0666);
    int shmid2 = shmget(key2 , 512 , IPC_CREAT | 0666);
   
    if (shmid1 == -1 || shmid2 == -1)
    {
        cout<<"couldnt make a SM "<<endl;
        return 0;
    }

	while (1)
    {
        char buf[BUF_LEN];
        struct sockaddr_in client;
        socklen_t clilen=sizeof(client);
        cout<<"receive"<<endl;
        recvfrom(rsfd,buf,BUF_LEN,0,(sockaddr*)&client,(socklen_t*)&clilen);

        struct iphdr* ip_header = (struct iphdr*)buf;
        char* ip_packet = buf;
        
        string ip_str;
        if (ip_header->protocol == IPPROTO_TCP) 
        {
            // Calculate the starting position of the TCP header
            unsigned int ip_header_length = ip_header->ihl * 4; // IP header length in bytes
            
            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(ip_header->saddr), ip, INET_ADDRSTRLEN);
            ip_str = ip;
            
            ips.insert(ip_str);
        }

        cout<<"TOtal clients are "<<ips.size()<<endl;
        // when you get 10 different vechiles 
        if (ips.size()==10)
        {   
            struct data * res1 = (struct data *)shmat(shmid1 , 0 , 0);

            // compare with the logs of B1 and B2 , and verify to continue or discontinue 
            int low = res1->lastindexread+1; int high = res1->lastindex;
            for (int i = low; i<=high ; i++)
            {
                string str = res1->messagee[i].ips;
                if (ips.find(str)==ips.end())
                {
                    kill(pidkill , SIGUSR1);
                    return 0;
                }
                else
                {
                    ips.erase(ips.find(str));
                }
            }
            res1->lastindexread = res1->lastindex;



            struct data * res2 = (struct data *)shmat(shmid2 , 0 , 0);

            low = res2->lastindexread+1; high = res2->lastindex;
            for (int i = low; i<=high ; i++)
            {
                string str = res2->messagee[i].ips;
                if (ips.find(str)==ips.end())
                {
                    kill(pidkill , SIGUSR1);
                    return 0;
                }
                else
                {
                    ips.erase(ips.find(str));
                }
            }
            res2->lastindexread = res2->lastindex;

            cout<<"All fine moji moji "<<endl;
        }
    }
    
}
