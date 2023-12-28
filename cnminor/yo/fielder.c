#include<time.h>
#include<stdio.h>
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
int main(int argc,char *argv[])
{
    int f=atoi(argv[1]);
    int rsfd=socket(AF_INET,SOCK_RAW,2);
	if(rsfd==-1)
	perror("socket");
    struct sockaddr_in client;
    socklen_t clilen=sizeof(client);
    printf("receive\n");
    int rsfd1=socket(AF_INET,SOCK_RAW,3);
	perror("socket");
	int optval=1;
	setsockopt(rsfd1, IPPROTO_IP, SO_BROADCAST, &optval, sizeof(int));
	printf("opt\n");
	struct sockaddr_in client1;
	client1.sin_family=AF_INET;
	client1.sin_addr.s_addr=inet_addr("127.0.0.1");
	client1.sin_addr.s_addr=INADDR_ANY;
	unsigned int client_len1=sizeof(client1);
	while(1)
    {
        char buf[1024];
        int r=recvfrom(rsfd,buf,1024,0,(struct sockaddr*)&client,(socklen_t*)&clilen);
        struct iphdr *ip;
        ip=(struct iphdr *)buf;
        int score=atoi(buf+ip->ihl*4);
        perror("recv");
        printf("Score : %d\n",score);
        if(f==10&&score<=15)
        {
            char buff1[]="Caught by fielder 10";
            sendto(rsfd1,buff1,strlen(buff1),0,(struct sockaddr*)&client,sizeof(client));
        }
        else if(f==20&&score>16&&score<=25)
        {
            char buff1[]="Caught by fielder 20";
            sendto(rsfd1,buff1,strlen(buff1),0,(struct sockaddr*)&client,sizeof(client));
        }
        else if(f==30&&score>26&&score<=35)
        {
            char buff1[]="Caught by fielder 30";
            sendto(rsfd1,buff1,strlen(buff1),0,(struct sockaddr*)&client,sizeof(client));
        }
        else
        {
            char buff1[]="Caught by fielder 40";
            sendto(rsfd1,buff1,strlen(buff1),0,(struct sockaddr*)&client,sizeof(client));
        }
    }
    return 0;
}