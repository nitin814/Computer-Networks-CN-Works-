#include<bits/stdc++.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include<arpa/inet.h>
#include<sys/un.h>
#include<csignal>
using namespace std;

int main()
{
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in addrr;
	
	addrr.sin_family=AF_INET;
	addrr.sin_addr.s_addr=inet_addr("127.0.0.1");
	addrr.sin_port=htons(3000);
	bind(sfd,(struct sockaddr*)&addrr,sizeof(addrr));
	listen(sfd,3);
    
	int len=sizeof(addrr);
	int nsfd=accept(sfd,(struct sockaddr*)&addrr,(socklen_t*)&len);
	
	int usfd = socket(AF_UNIX , SOCK_DGRAM , 0);
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path , "/tmp/socketfile");

    char cmsg[CMSG_SPACE(sizeof(int))];

    struct iovec e = {NULL , 0};

    struct msghdr msg = {(void *)&addr, sizeof(addr), &e , 1 , cmsg , sizeof(cmsg) , 0};

    struct cmsghdr * cmsgptr = CMSG_FIRSTHDR(&msg);
    cmsgptr->cmsg_level = SOL_SOCKET;
    cmsgptr->cmsg_type = SCM_RIGHTS;
    cmsgptr->cmsg_len = CMSG_LEN(sizeof(int));
    *(int *)CMSG_DATA(cmsgptr) = nsfd;
    int n;
    if((n = sendmsg(usfd, &msg, 0))<0)
    {
        perror("sendmsg err");
        exit(1);
    }
    cout<<"naman"<<n<<endl;
    close(nsfd);
    cout<<"sent sfd"<<endl;
    
    char buffer[1024];
    cout<<"waiting for message "<<endl;
    socklen_t addrlen = sizeof(addr);
    ssize_t bytes_received = recvfrom(usfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&addr, &addrlen);
    if (bytes_received == -1) 
    {
        perror("recvfrom");
        close(usfd);
        return 1;
    }
    cout<<"got message "<<bytes_received<<endl;
    cout<<buffer<<endl; 

    while (1)
    {
        char buffer[1024];
        cout<<"waiting for message "<<endl;
        socklen_t addrlen = sizeof(addr);
        ssize_t bytes_received = recvfrom(usfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&addr, &addrlen);
        if (bytes_received == -1) 
        {
            perror("recvfrom");
            close(usfd);
            return 1;
        }
        cout<<"got message "<<bytes_received<<endl;
        cout<<buffer<<endl; 
    }
	return 0;
}
