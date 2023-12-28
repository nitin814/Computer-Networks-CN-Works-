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

struct messagetoken
{
    int id;
};

int main() 
{
    int rsfd = socket(AF_INET, SOCK_RAW , 10);
    if(rsfd==-1)
    {
        cout<<"ypypy"<<endl;
        return 0;
    }

	while (1)
    {
        char buf[BUF_LEN];
        struct sockaddr_in client;
        socklen_t clilen=sizeof(client);
        recvfrom(rsfd,buf,BUF_LEN,0,(sockaddr*)&client,(socklen_t*)&clilen);

        struct iphdr* ip_header = (struct iphdr*)buf;
        unsigned int ip_header_length = ip_header->ihl * 4; 
        struct tcphdr* tcp_header = (struct tcphdr*)(buf + ip_header_length);

        char* ip_packet = buf; 
        ip_header_length = (ip_packet[0] & 0x0F) * 4; // Calculate IP header length
        unsigned int tcp_data_offset = ip_packet[ip_header_length + 12] >> 4; // Extract the Data Offset field
        unsigned int tcp_header_length = tcp_data_offset * 4; // Multiply by 4 to get the length in bytes

        unsigned int total_length = ntohs(reinterpret_cast<uint16_t*>(ip_packet + 2)[0]);
    
        struct messagetoken* token = (struct messagetoken*)(ip_packet + ip_header_length + tcp_header_length);

        if (token->id != 1)
        {
            continue;
        }

        unsigned int payload_length = total_length - ip_header_length - tcp_header_length - sizeof(struct messagetoken);

        char* payload = ip_packet + ip_header_length + tcp_header_length + sizeof(struct messagetoken);
        string str (payload , payload + payload_length);
        stringstream s (str);
        string word;
        
        s>>word;
        if (word[0]>='0' && word[0]<='9')
        {
            int sfd = socket(AF_INET , SOCK_STREAM , 0);
            if (sfd==-1)
            {
                cout<<"Error in creating socket"<<endl;
                return 1;
            }

            struct sockaddr_in serveraddress;
            serveraddress.sin_family = AF_INET;
            serveraddress.sin_port = htons(stoi(word));
            serveraddress.sin_addr.s_addr = inet_addr("127.0.0.0");

            int t = connect (sfd , (sockaddr *)&serveraddress , sizeof(serveraddress));
            if (t==-1)
            {
                cout<<"Error in connecting "<<endl;
                return 1;
            }

            string remaining = str.substr(word.length()+1);
            const char * buf = remaining.c_str();
    
            cout<<"sending it to the live server "<<endl;
            send(sfd , buf , strlen(buf)+1 , 0);
            cout<<"sended to live server"<<endl;
        }
        else
        {
            if (word.length()>0)
                cout<<"read the news by news: "<<word<<endl;
        }
    }
}
