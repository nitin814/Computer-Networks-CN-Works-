#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include <sys/wait.h>
#include<poll.h>
#include<sys/msg.h>
#include<csignal>
#include<sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/un.h>
using namespace std;

int main()
{
    int usfd=socket(AF_UNIX,SOCK_DGRAM,0);
  
    struct sockaddr_un addr;
    memset(&addr,0,sizeof(addr));
    addr.sun_family=AF_UNIX;
    unlink("pth");
    strcpy(addr.sun_path,"pth");
    
    bind(usfd,(struct sockaddr*)&addr,sizeof(addr));
    
    char buff[100];
        
    struct iovec iov={buff,100};
    char cmsg[CMSG_SPACE(sizeof(int))];
    struct msghdr msg={NULL,0,&iov,1,cmsg,sizeof(cmsg),0};
    
    cout<<"waiting for fd from p1 "<<endl; 
    
    recvmsg(usfd,&msg,0);
    
    struct cmsghdr *c = CMSG_FIRSTHDR(&msg);
    
    int nsfd= *(int*)CMSG_DATA(c); 
    cout<<"recieved fd:"<<nsfd<<endl;
    socklen_t llen=sizeof(addr);
  
    while(1)
    {
        char buff[100];
        recv(nsfd,buff,sizeof(buff),0);
        cout<<"recieved from client:"<<buff<<endl;
        sendto(usfd,buff,sizeof(buff),0,(struct sockaddr*)&addr,sizeof(addr)); 
    }
}