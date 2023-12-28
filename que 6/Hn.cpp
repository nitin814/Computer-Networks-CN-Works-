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
#include <netinet/udp.h>
#define BUF_LEN 65000

using namespace std;

int sfd;
vector<int> friends;
int udpSocket;
int flag = 0;

// accept the connections from friends
void * func1 (void * args)
{
    while (1)
    {
        struct sockaddr_in clientaddr;
        socklen_t len = sizeof(clientaddr);
        int newfd = accept(sfd , (struct sockaddr*)&clientaddr , &len);
        if (newfd == -1)
        {
            cout<<"not"<<endl;
            continue;
        }

        friends.push_back(newfd);
    }
}

void sendUdpReply(const char* replyMessage, const char* destIp, uint16_t destPort, const char* sourceIp, uint16_t sourcePort) 
{
    sockaddr_in destAddr{};
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(destPort);
    inet_pton(AF_INET, destIp, &(destAddr.sin_addr));

    if (flag==0)
    {
        sockaddr_in sourceAddr{};
        sourceAddr.sin_family = AF_INET;
        sourceAddr.sin_port = htons(sourcePort);
        inet_pton(AF_INET, sourceIp, &(sourceAddr.sin_addr));

        // Bind the socket to the source IP
        if (bind(udpSocket, reinterpret_cast<struct sockaddr*>(&sourceAddr), sizeof(sourceAddr)) == -1) 
        {
            perror("Error binding socket to source IP");
            close(udpSocket);
            exit(EXIT_FAILURE);
        }
        flag = 1;
    }

    // Send the UDP reply packet
    if (sendto(udpSocket, replyMessage, strlen(replyMessage), 0, reinterpret_cast<struct sockaddr*>(&destAddr), sizeof(destAddr)) == -1) 
    {
        perror("Error sending UDP reply packet");
        close(udpSocket);
        exit(EXIT_FAILURE);
    }

    cout << "UDP reply packet sent successfully." << endl;
}

void *func2(void *args) 
{
    int rsfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (rsfd == -1) 
    {
        cerr << "Error in creating raw socket" << endl;
        return nullptr;
    }

    while (1) 
    {
        unsigned char buffer[BUF_LEN];
        ssize_t bytesRead = recv(rsfd, buffer, BUF_LEN, 0);

        if (bytesRead == -1) 
        {
            perror("Error receiving packet");
            close(rsfd);
            return NULL;
        }

        struct iphdr* ipHeader = (struct iphdr*)(buffer);

        if (ipHeader->protocol == IPPROTO_UDP) 
        {
            struct udphdr* udpHeader = (struct udphdr*)(buffer + (ipHeader->ihl << 2));

            int udpHeaderSize = sizeof(struct udphdr);
            int ipHeaderSize = (ipHeader->ihl) << 2;
            int payloadSize = bytesRead - ipHeaderSize - udpHeaderSize; // Fix: use bytesRead instead of size

            // Extract source IP
            char sourceIp[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(ipHeader->saddr), sourceIp, INET_ADDRSTRLEN);

            // Extract destination IP
            char destIp[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(ipHeader->daddr), destIp, INET_ADDRSTRLEN);

            // Extract source port
            uint16_t sourcePort = ntohs(udpHeader->source);
            // Extract destination port
            uint16_t destPort = ntohs(udpHeader->dest);

            // Extract payload
            if (payloadSize > 0) 
            {
                const unsigned char* payload = buffer + ipHeaderSize + udpHeaderSize;

                string str(reinterpret_cast<const char*>(payload), payloadSize);

                // Use random logic to select a friend from the vector 'friends'
                if (!friends.empty()) 
                {
                    int random = rand() % friends.size();
                    int fd = friends[random];

                    cout << "Sending to fd" << endl;
                    send(fd, str.c_str(), str.size(), 0);

                    char buf2[1024];
                    recv(fd, buf2, sizeof(buf2), 0);
                    cout << "Received answer from friend, sending it now..." << endl;

                    // send back to mncr , as per behaviour of ot
                    sendUdpReply(buf2 , sourceIp, sourcePort, destIp, destPort); 
                }
            }
        }
        
    }
}

int main ()
{
    // raw socket for accepting connections from friends
    sfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sfd==-1)
    {
        cout<<"Error in creating socket"<<endl;
        return 1;
    }

    struct sockaddr_in serveraddress;
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(9999);
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

    // raw socket for sending udp packets
    udpSocket = socket(AF_INET, SOCK_DGRAM, 0);

    if (udpSocket == -1) 
    {
        perror("Error creating UDP socket");
        exit(EXIT_FAILURE);
    }

    pthread_t t1;
    pthread_t t2;

    pthread_create(&t1 , NULL , func1 , NULL);
    pthread_create(&t2 , NULL , func2 , NULL);
    pthread_join(t1 , NULL);
    pthread_join(t2 , NULL);
}
