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
#define BUF_LEN 65000

int rsfd = 0;
int rsfdrecv = 0;

// sending the input to the clients of channels ... by producing data ...
void * thread1 (void * args)
{   
    struct sockaddr_in client;
	client.sin_family=AF_INET;
	client.sin_addr.s_addr=inet_addr("127.0.0.1");

    while (1)
    {
        string str;
        getline(cin , str);
        
        const char * buff = str.c_str();

        unsigned int client_len=sizeof(client);
        cout<<"sending to my channels "<<endl;
        sendto(rsfd,buff,strlen(buff)+1,0,(struct sockaddr*)&client,sizeof(client));
        cout<<"sended "<<endl;
    }
}

// getting the data from the manager or advertisment , accept it and send output to the clients ...
void * thread2 (void * args)
{
    struct sockaddr_in client;
	client.sin_family=AF_INET;
	client.sin_addr.s_addr=inet_addr("127.0.0.1");

    while (1)
    {
        char buf[BUF_LEN];
        struct sockaddr_in client;
        socklen_t clilen=sizeof(client);
        cout<<"receive"<<endl;
        recvfrom(rsfdrecv,buf,BUF_LEN,0,(sockaddr*)&client,(socklen_t*)&clilen);
    
        struct iphdr *ip;
        ip=(struct iphdr*)buf;
        const char * buff = (buf+(ip->ihl)*4);
        
        unsigned int client_len=sizeof(client);
        cout<<"sending to my channels "<<endl;
        sendto(rsfd,buff,strlen(buff)+1,0,(struct sockaddr*)&client,sizeof(client));
    }
}

int main ()
{
    rsfd = socket(AF_INET,SOCK_RAW,10);
    if(rsfd<0)
    {
        cout<<"Socket Error\n";
        return 1;
    }
    
    rsfdrecv = socket(AF_INET, SOCK_RAW , 20);
    if(rsfdrecv==-1)
    {
        cout<<"ypypy"<<endl;
        return 0;
    }

    pthread_t t1;
    pthread_t t2;
    pthread_create(&t1 , NULL , thread1 , NULL);
    pthread_create(&t2 , NULL , thread2 , NULL);

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
}