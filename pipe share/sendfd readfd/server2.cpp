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
#include <sys/uio.h>


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

int main ()
{

	int usfd = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "socket");

    int connectt = connect(usfd, (struct sockaddr*)&addr, sizeof(addr));
    if(connectt < 0)
    {
        cout<<"Connect Error\n";
        exit(0);
    }

    int ofd = recvfd(usfd); cout<<"got the fd"<<endl;

    while (1)
	{
		char buf[100];
		int n = read(ofd, buf, 100);
		cout<<buf<<endl;
		
		cout<<"now sending that thang "<<endl;
		send(usfd, buf, n, 0);
		cout<<"sent : "<<buf<<endl;
	}
}