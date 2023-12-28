#include<bits/stdc++.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include<sys/un.h>
#include<csignal>

using namespace std;

int main()
{
	int usfd = socket(AF_UNIX , SOCK_DGRAM , 0);
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    unlink("/tmp/socketfile");
    strcpy(addr.sun_path , "/tmp/socketfile");

    int binderror = bind(usfd , (struct sockaddr*)&addr , sizeof(addr));
    if (binderror < 0)
    {
        cout << "Bind error\n";
        return 1;
    }

    cout<<"ok"<<endl;
    char cmsg[CMSG_SPACE(sizeof(int))];

    char buf[512];
    struct iovec e = {buf , 512};
    
    struct msghdr msg = {(void *)&addr, sizeof(addr), &e , 1 , cmsg , sizeof(cmsg) , 0};
    cout<<"ok"<<endl;
    int n = recvmsg(usfd , &msg , 0);
    cout<<"got some message "<<n<<endl;

    struct cmsghdr * cmsgptr = CMSG_FIRSTHDR(&msg);
    int nsfd = *(int *)CMSG_DATA(cmsgptr);
    while (1)
    {
        char buf1[100];
        recv(nsfd,buf1,sizeof(buf),0);
        cout<<buf1<<endl;
        cout<<"sending message now to p1"<<endl;

        int usfd = socket(AF_UNIX , SOCK_DGRAM , 0);
        struct sockaddr_un addr;
        addr.sun_family = AF_UNIX;
        unlink("/tmp/socketfile");
        strcpy(addr.sun_path , "/tmp/socketfile");

        int binderror = bind(usfd , (struct sockaddr*)&addr , sizeof(addr));
        if (binderror < 0)
        {
            cout << "Bind error\n";
            return 1;
        }

        ssize_t bytes_sent = sendto(usfd, buf1, strlen(buf1), 0,(struct sockaddr*)&addr, sizeof(addr));
        cout<<bytes_sent<<endl;
        if (bytes_sent == -1) 
        {
            perror("sendto");
            close(usfd);
            return 1;
        }
        cout<<"sent data"<<endl;
    }
    
	return 0;
}

