#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <sys/wait.h>
#include <poll.h>
#include <sys/un.h>
using namespace std;

int main ()
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
    int iofd = open("in.txt" , O_RDONLY);
    cout<<"ok"<<endl;
    char cmsg[CMSG_SPACE(sizeof(int))];
  
    struct iovec e = {NULL , 0};
    
    struct msghdr msg = {(void *)&addr, sizeof(addr), &e , 1 , cmsg , sizeof(cmsg) , 0};
    cout<<"ok"<<endl;
    int n = recvmsg(usfd , &msg , 0);
    cout<<"got some message "<<n<<endl;
    
    struct cmsghdr * cmsgptr = CMSG_FIRSTHDR(&msg);
    int fd = *(int *)CMSG_DATA(cmsgptr);

    cout<<"got fd as : "<<fd<<endl;
    



    char cmsg2[CMSG_SPACE(sizeof(int))];
  
    e = {NULL , 0};
    
    struct msghdr msg2 = {(void *)&addr, sizeof(addr), &e , 1 , cmsg2 , sizeof(cmsg2) , 0};
    cout<<"ok"<<endl;
    n = recvmsg(usfd , &msg2 , 0);
    cout<<"got some message "<<n<<endl;
    
    cmsgptr = CMSG_FIRSTHDR(&msg2);
    int ofd = *(int *)CMSG_DATA(cmsgptr);

    cout<<"got fd as : "<<ofd<<endl;    

    char bufe[100];
    n = read(ofd , bufe , sizeof(bufe));
    bufe[n] = '\0';
    cout<<"read on the second time : "<<bufe<<endl;
}