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
    strcpy(addr.sun_path , "/tmp/socketfile");
   
    cout<<"ok"<<endl; int ifddd = open("in.txt" , O_RDONLY);

    char cmsg[CMSG_SPACE(sizeof(int))];
    
    struct iovec e = {NULL , 0}; int ifd = open("in.txt" , O_RDONLY);

    struct msghdr msg = {(void *)&addr, sizeof(addr), &e , 1 , cmsg , sizeof(cmsg) , 0};
  
    struct cmsghdr * cmsgptr = CMSG_FIRSTHDR(&msg);
    cmsgptr->cmsg_len = CMSG_LEN(sizeof(int));
    cmsgptr->cmsg_level = SOL_SOCKET;
    cmsgptr->cmsg_type = SCM_RIGHTS;
    *(int *)CMSG_DATA(cmsgptr) = ifd;
    
    cout<<"ok , sending the fd : "<<ifd<<endl;
    int n = sendmsg(usfd , &msg , 0);
    cout<<"sent some message "<<n<<endl;




    int ofd = open("next.txt" , O_RDONLY);
    char cmsg2[CMSG_SPACE(sizeof(int))];
    
    e = {NULL , 0}; 

    struct msghdr msg2 = {(void *)&addr, sizeof(addr), &e , 1 , cmsg2 , sizeof(cmsg2) , 0};
  
    cmsgptr = CMSG_FIRSTHDR(&msg2);
    cmsgptr->cmsg_len = CMSG_LEN(sizeof(int));
    cmsgptr->cmsg_level = SOL_SOCKET;
    cmsgptr->cmsg_type = SCM_RIGHTS;
    *(int *)CMSG_DATA(cmsgptr) = ofd;
    
    cout<<"ok , sending the fd : "<<ofd<<endl;
    n = sendmsg(usfd , &msg2 , 0);  
    cout<<"sent some message "<<n<<endl;




}