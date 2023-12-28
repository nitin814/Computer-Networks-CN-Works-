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

using namespace std;

int sendfd(int s, int fd)
{
	char buf[1];
	struct iovec iov;
	struct msghdr msg;
	struct cmsghdr *cmsg;
	int n;
	char cms[CMSG_SPACE(sizeof(int))];

	buf[0] = 0;
	iov.iov_base = buf;
	iov.iov_len = 1;

	memset(&msg, 0, sizeof msg);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = (caddr_t)cms;
	msg.msg_controllen = CMSG_LEN(sizeof(int));

	cmsg = CMSG_FIRSTHDR(&msg);
	cmsg->cmsg_len = CMSG_LEN(sizeof(int));
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	memmove(CMSG_DATA(cmsg), &fd, sizeof(int));

	if((n=sendmsg(s, &msg, 0)) != iov.iov_len)
		return -1;
	return 0;
}

int main ()
{
	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addrr;
	addrr.sin_family = AF_INET;
	addrr.sin_port = htons(8080);
	addrr.sin_addr.s_addr = INADDR_ANY;

	int binderror = bind(sfd, (struct sockaddr*)&addrr, sizeof(addrr));
	if(binderror < 0)
	{
		cout<<"Bind Error\n";
		exit(0);
	}

	int listenerror = listen(sfd, 5);

	int usfd = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    unlink("socket");
    strcpy(addr.sun_path, "socket");

    binderror = bind(usfd, (struct sockaddr*)&addr, sizeof(addr));
    if(binderror < 0)
    {
        cout<<"Bind Error\n";
        exit(0);
    }

    listenerror = listen(usfd, 5);
    if(listenerror < 0)
    {
        cout<<"Listen Error\n";
        exit(0);
    }

	int nsfd = accept(sfd, NULL, NULL);

    int nusfd = accept(usfd, NULL, NULL);

    sendfd(nusfd, nsfd);
    cout<<"sent the fd"<<endl;

	while (1)
	{
		char buf[100];
		int n = read(nusfd, buf, 100);
		cout<<"got smthg!"<<endl;
		cout<<buf<<endl;
	}
}