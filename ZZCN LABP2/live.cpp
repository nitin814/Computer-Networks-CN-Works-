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

int main ()
{
    int sfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sfd==-1)
    {
        cout<<"Error in creating socket"<<endl;
        return 1;
    }

    struct sockaddr_in serveraddress;
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(3000);
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.0");

    int t = connect (sfd , (sockaddr *)&serveraddress , sizeof(serveraddress));
    if (t==-1)
    {
        cout<<"Error in connecting "<<endl;
        return 1;
    }

    while (1)
    {
        string str; 
        getline(cin , str);
        const char * buff = str.c_str();
        send (sfd , buff , strlen(buff)+1 , 0);
        cout<<"I am liver and i sended "<<buff<<endl;
    }
}