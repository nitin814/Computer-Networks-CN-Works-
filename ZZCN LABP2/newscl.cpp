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
using namespace std;
#define BUF_LEN 65000

using namespace std;

int main() 
{
    int rsfd = socket(AF_INET, SOCK_RAW , 10);
    if(rsfd==-1)
    {
        cout<<"ypypy"<<endl;
        return 0;
    }
    // extract payload and enjoy watching it ...
	while (1)
    {
        char buf[BUF_LEN];
        // struct sockaddr_in client;
        // socklen_t clilen=sizeof(client);
        // cout<<"receive"<<endl;
        // recvfrom(rsfd,buf,BUF_LEN,0,(sockaddr*)&client,(socklen_t*)&clilen);
       
        // struct iphdr *ip;
        // ip=(struct iphdr*)buf;
        // cout<<"I am client of newschannel and i am listening : "<<(buf+(ip->ihl)*4)<<endl;
       
        struct sockaddr_in client;
        socklen_t clilen=sizeof(client);
        cout<<"receive"<<endl;
        recvfrom(rsfd,buf,BUF_LEN,0,(sockaddr*)&client,(socklen_t*)&clilen);
    
        struct iphdr* ip_header = (struct iphdr*)buf;
        unsigned int ip_header_length = ip_header->ihl * 4; 
        // struct tcphdr* tcp_header = (struct tcphdr*)(buf + ip_header_length);

        char* ip_packet = buf; 
        ip_header_length = (ip_packet[0] & 0x0F) * 4; // Calculate IP header length
        unsigned int tcp_data_offset = ip_packet[ip_header_length + 12] >> 4; // Extract the Data Offset field
        unsigned int tcp_header_length = tcp_data_offset * 4; // Multiply by 4 to get the length in bytes
        // cout<<tcp_header_length<<" is the length "<<endl;
        unsigned int total_length = ntohs(reinterpret_cast<uint16_t*>(ip_packet + 2)[0]);
        unsigned int payload_length = total_length - ip_header_length - tcp_header_length;

        char* payload = ip_packet + ip_header_length + tcp_header_length;
        cout<<"getting something : "<<endl;
        for (int i = 0;i<payload_length;i++)
        {
            cout<<*(payload+i);
        }
        cout<<endl;
    }
}
