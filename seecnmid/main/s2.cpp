#include <bits/stdc++.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <csignal>
#include <arpa/inet.h>
#include <poll.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <sys/wait.h>
#include <pthread.h>

using namespace std;

int recvfd(int s)
{
	int n;
	int fd;
	char buf[1];
	struct iovec iov;
	struct msghdr msg;
	struct cmsghdr *cmsg;
	char cms[CMSG_SPACE(sizeof(int))];

	iov.iov_base = buf;
	iov.iov_len = 1;

	memset(&msg, 0, sizeof msg);
	msg.msg_name = 0;
	msg.msg_namelen = 0;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;

	msg.msg_control = (caddr_t)cms;
	msg.msg_controllen = sizeof cms;

	if((n=recvmsg(s, &msg, 0)) < 0)
		return -1;
	if(n == 0){
		cout<<"whay!!!"<<endl;
		return -1;
	}
	cmsg = CMSG_FIRSTHDR(&msg);
	memmove(&fd, CMSG_DATA(cmsg), sizeof(int));
	return fd;
}

void * service (void * args)
{
    int nsfd = *(int*)args;
    while (1)
    {
        char buff[100];
        int n = recv(nsfd, buff, 100, 0);
        buff[n] = '\0';
        cout<<buff<<endl;
    }
}

int main ()
{
    int usfd = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "socketfile");

    int ret = connect(usfd, (struct sockaddr*)&addr, sizeof(addr));
    if (ret<0)
    {
        perror("connect");
        exit(0);
    }

    int flag = 0;
    while (1)
    {
        int fd = recvfd(usfd);
        if (flag==0)
        {
            pthread_t id;
            pthread_create(&id,NULL,&service,(void*)&fd);
        }
        else
        {
            int c= 0 ;
            c = fork ();
            if (c==0)
            {
                while (1)
                {
                    char buff[100];
                    int n = recv(fd, buff, 100, 0);
                    buff[n] = '\0';
                    cout<<buff<<endl;
                }
            }
        }
    }
}