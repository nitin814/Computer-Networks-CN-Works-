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
    int fd = open("fors44.txt" , O_WRONLY);

    cout<<"naman"<<endl;

    cout<<"ok"<<endl;
    struct pollfd pfd[4]; vector<int> passfd(4);

    for (int i=0;i<4;i++)
    {
        int sfd = socket(AF_INET, SOCK_STREAM, 0);

        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(8080+i);
        addr.sin_addr.s_addr = INADDR_ANY;

        int binderror = bind(sfd, (struct sockaddr*)&addr, sizeof(addr));
        if (binderror == -1)
        {
            cout << "Bind error\n";
            exit(0);
        }

        int listenerror = listen(sfd, 5);

        pfd[i].fd = sfd;
        pfd[i].events = POLLIN;
    }



    int usfd = socket(AF_UNIX, SOCK_STREAM, 0);

    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    unlink("socketfile");
    strcpy(addr.sun_path, "socketfile");

    int binderror = bind(usfd, (struct sockaddr*)&addr, sizeof(addr));
    if (binderror == -1)
    {
        cout << "Bind error\n";
        exit(0);
    }
    int listenerror = listen(usfd, 5);

    cout<<"waiting for all servers to get connected"<<endl;
    for (int i=0;i<4;i++)
    {
        int nsfd = accept(usfd, NULL, NULL);
        cout<<"got one !"<<endl;
        passfd[i] = nsfd;

        if (i==3)
        {
            int reuse = 1;
            if (setsockopt(passfd[3], SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
            perror("setsockopt(SO_REUSEADDR) failed");
            if (setsockopt(passfd[3], SOL_SOCKET, SO_REUSEPORT, (const char*)&reuse, sizeof(reuse)) < 0) 
            perror("setsockopt(SO_REUSEPORT) failed");  

            cout<<"sending "<<passfd[3]<<endl;
            string fdsend = to_string(passfd[3]);
            const char * c = fdsend.c_str();
            write(fd , c , strlen(c));
            cout<<"wrote that thang "<<endl;    
        }
    }

    cout<<"all servers connected "<<endl;
    
    while (1)
    {
        int s = poll(pfd, 4, 1000);
        if (s<=0)
            continue;

        for (int i=0;i<4;i++)
        {
            if (i!=4 && (pfd[i].revents & POLLIN))
            {
                int nsfd = accept(pfd[i].fd, NULL, NULL);
                
                cout<<"got the cleint for "<<i<<endl;
                cout<<"passing this to server "<<i<<endl;

                sendfd(passfd[i] , nsfd);
                cout<<"sended "<<endl;
            }
            if (i==4 && (pfd[i].revents & POLLIN))
            {
                cout<<"sending signal to s4 "<<endl;
                int pid; cin>>pid;
                kill(pid, SIGUSR1);
            }
        }
    }
}