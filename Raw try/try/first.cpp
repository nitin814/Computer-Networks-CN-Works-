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
void print_ipheader(struct iphdr* ip)
{
	cout<<"------------------------\n";
	cout<<"Printing IP header....\n";
	cout<<"IP version:"<<(unsigned int)ip->version<<endl;
	cout<<"IP header length:"<<(unsigned int)ip->ihl<<endl;
	
	cout<<"Type of service:"<<(unsigned int)ip->tos<<endl;
	cout<<"Total ip packet length:"<<ntohs(ip->tot_len)<<endl;
	cout<<"Packet id:"<<ntohs(ip->id)<<endl;
	cout<<"Time to leave :"<<(unsigned int)ip->ttl<<endl;
	cout<<"Protocol:"<<(unsigned int)ip->protocol<<endl;
	cout<<"Check:"<<ip->check<<endl;
	cout<<"Source ip:"<<inet_ntoa(*(in_addr*)&ip->saddr)<<endl;
	//printf("%pI4\n",&ip->saddr );
	cout<<"Destination ip:"<<inet_ntoa(*(in_addr*)&ip->daddr)<<endl;
	cout<<"End of IP header\n";
	cout<<"------------------------\n";
}

int main() 
{
    int rsfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if(rsfd==-1)
    {
        cout<<"ypypy"<<endl;
        return 0;
    }

	char buf[BUF_LEN];
	struct sockaddr_in client;
	socklen_t clilen=sizeof(client);
	cout<<"receive"<<endl;
	recvfrom(rsfd,buf,BUF_LEN,0,(sockaddr*)&client,(socklen_t*)clilen);
	perror("recv");

    struct iphdr* ip_header = (struct iphdr*)buf;
    print_ipheader(ip_header); char* ip_packet = buf;
    
    if (ip_header->protocol == IPPROTO_TCP) 
    {
        // Calculate the starting position of the TCP header
        unsigned int ip_header_length = ip_header->ihl * 4; // IP header length in bytes
        cout<<"ip header length is "<<ip_header_length<<endl;
        struct tcphdr* tcp_header = (struct tcphdr*)(buf + ip_header_length);

        cout<<" size of tcp : "<<sizeof(tcp_header)<<endl;
        // Access and print TCP header fields
        std::cout << "Source Port: " << ntohs(tcp_header->source) << std::endl;
        std::cout << "Destination Port: " << ntohs(tcp_header->dest) << std::endl;
        std::cout << "Sequence Number: " << ntohl(tcp_header->seq) << std::endl;
        std::cout << "Acknowledgment Number: " << ntohl(tcp_header->ack_seq) << std::endl;
        std::cout << "Data Offset: " << (unsigned int)tcp_header->doff << std::endl;
        std::cout << "ACK Flag: " << (unsigned int)tcp_header->ack << std::endl;
        std::cout << "SYN Flag: " << (unsigned int)tcp_header->syn << std::endl;
        std::cout << "Window: " << ntohs(tcp_header->window) << std::endl;
        std::cout << "Checksum: " << ntohs(tcp_header->check) << std::endl;
        std::cout << "Urgent Pointer: " << ntohs(tcp_header->urg_ptr) << std::endl;

        char* ip_packet = buf; 
        ip_header_length = (ip_packet[0] & 0x0F) * 4; // Calculate IP header length
        unsigned int tcp_data_offset = ip_packet[ip_header_length + 12] >> 4; // Extract the Data Offset field
        unsigned int tcp_header_length = tcp_data_offset * 4; // Multiply by 4 to get the length in bytes


        unsigned int total_length = ntohs(reinterpret_cast<uint16_t*>(ip_packet + 2)[0]);
        unsigned int payload_length = total_length - ip_header_length - tcp_header_length;

        cout<<"lengths of ip and tcp are : "<<ip_header_length<<" and "<<tcp_header_length<<endl;
        cout<<"total length is "<<total_length<<endl;

        char* payload = ip_packet + ip_header_length + tcp_header_length;

        for (int i = 0;i<payload_length;i++)
        {
            cout<<*(payload+i);
        }
        cout<<endl;
        cout<<"is the data"<<endl;
    }
}
