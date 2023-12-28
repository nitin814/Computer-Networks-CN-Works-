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


void * func (void * args)
{
    int ofd = *(int*)args;
    while (1)
    {
        char buff[100];
        int n = recv(ofd, buff, 100, 0);
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

    while (1)
    {
        int nusfd = recvfd(usfd);
        pthread_t tid;
        pthread_create(&tid, NULL, func, (void*)&nusfd);
        pthread_join(tid, NULL);
    } 
}