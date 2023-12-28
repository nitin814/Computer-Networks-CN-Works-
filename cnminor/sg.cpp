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
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>

using namespace std;
#define BUF_LEN 65000

vector<string> blockedip = {"127.0.0.1"};
struct iphdr *ip_header;
map<uint32_t, uint8_t[ETH_ALEN]> arpTable;

void processArpPacket(const char* buffer, ssize_t length) 
{
    struct ether_arp* arpPacket = (struct ether_arp*)(buffer + ETH_HLEN);

    // Check if it's an ARP reply
    if (ntohs(arpPacket->ea_hdr.ar_op) == ARPOP_REPLY) 
    {
        uint32_t senderIP = *(reinterpret_cast<uint32_t*>(&arpPacket->arp_spa));
        uint8_t senderMAC[ETH_ALEN];
        memcpy(senderMAC, arpPacket->arp_sha, ETH_ALEN);

        // Check if sender's MAC matches the one in the ARP table
        auto it = arpTable.find(senderIP);
        if (it != arpTable.end() && memcmp(senderMAC, it->second, ETH_ALEN) != 0) 
        {
            cout << "Possible ARP spoofing detected!" << endl;
            cout<<"Blocking spoofer for safeguarding "<<endl;
        }
        memcpy(arpTable[senderIP], senderMAC, ETH_ALEN);
    }
}

void signalHandler(int signo, siginfo_t *info, void *context) 
{
    cout<<"yoyoy"<<endl;
    int sigaction = info->si_pid;
    string sourceIP = inet_ntoa(*(struct in_addr*)&(ip_header->saddr));
    cout<<"sourceIP"<<sourceIP<<endl;
    if (find(blockedip.begin() , blockedip.end() , sourceIP) == blockedip.end())
    {
        kill (sigaction , SIGUSR1); // it is non blocked , you can continue accepting message 
    }
    else
        kill (sigaction , SIGUSR2); 
}

void * arpThread (void * args)
{
    int sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if (sockfd == -1) 
    {
        perror("Socket creation error");
        return NULL;
    }    

    while (1) 
    {
        char buffer[2048];
        ssize_t length = recvfrom(sockfd, buffer, sizeof(buffer), 0, nullptr, nullptr);
        if (length == -1) 
        {
            perror("Packet receive error");
            close(sockfd);
            return NULL;
        }

        processArpPacket(buffer, length);
    }   
    return NULL;
}

void * tcpThread (void * args)
{
    int rsfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if(rsfd==-1)
    {
        cout<<"ypypy"<<endl;
        return NULL;
    }
    cout<<"connnectd"<<endl;

    while (1)
    {
        char buf[BUF_LEN];
        struct sockaddr_in client;
        socklen_t clilen=sizeof(client);
        recvfrom(rsfd,buf,BUF_LEN,0,(sockaddr*)&client,(socklen_t*)&clilen);
        ip_header = (struct iphdr*)buf;
        sleep(2);

        string sourceIP = inet_ntoa(*(struct in_addr*)&(ip_header->saddr));
        if (find(blockedip.begin() , blockedip.end() , sourceIP) != blockedip.end())
        {
            int usfd = socket(AF_INET , SOCK_DGRAM , 0);
            if (usfd==-1)
            {
                cout<<"Error in creating socket"<<endl;
                return NULL;
            }

            struct sockaddr_in serveraddress;
            serveraddress.sin_family = AF_INET;
            serveraddress.sin_port = htons(9999);
            serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
            char buffer[100] = "HACKER!";
            sendto(usfd , buffer , strlen(buffer)+1 , 0 , (struct sockaddr *)&serveraddress , sizeof(serveraddress));
        }
    }
    return NULL;
}

int main ()
{
    cout<<"I am sg with pid of "<<getpid()<<endl;
    struct sigaction sa;
    sa.sa_sigaction = signalHandler;
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &sa, NULL);

    pthread_t arps;
    pthread_t tcps;

    pthread_create(&arps, NULL, arpThread, NULL);
    pthread_create(&tcps, NULL, tcpThread, NULL);

    pthread_join(arps, NULL);
    pthread_join(tcps, NULL);
}