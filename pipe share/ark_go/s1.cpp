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
    int sfd=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in my_addr;
    my_addr.sin_family=AF_INET;
    my_addr.sin_addr.s_addr=inet_addr("127.0.0.5");
    my_addr.sin_port=htons(8002);
    
    bind(sfd,(struct sockaddr*)&my_addr,sizeof(my_addr));
    listen(sfd,5);
    socklen_t len=sizeof(my_addr);
    int nsfd= accept(sfd,(struct sockaddr*)&my_addr,&len);
    cout<<"accepeted"<<endl;

    int usfd=socket(AF_UNIX,SOCK_DGRAM,0);
  
    struct sockaddr_un addr,addr2;
    memset(&addr,0,sizeof(addr));
    addr.sun_family=AF_UNIX;
    strcpy(addr.sun_path,"pth");
    char buff[1024];
    struct iovec iov={buff,100};
    char cmsg[CMSG_SPACE(sizeof(int))];
    struct msghdr msg={&addr,sizeof(addr),&iov,1,cmsg,sizeof(cmsg),0};
    struct cmsghdr *c = CMSG_FIRSTHDR(&msg);
    c->cmsg_level = SOL_SOCKET;
    c->cmsg_type = SCM_RIGHTS;
    c->cmsg_len = CMSG_LEN(sizeof(int));
    *(int*)CMSG_DATA(c) =nsfd;
    cout<<"sending fd:"<<nsfd<<endl;

    sendmsg(usfd,&msg,0);
    

    int f=0;
    while(1)
    {
        char a[1024],b[100];
        
        socklen_t llen=sizeof(addr);
        if(f==0)
        {
            f=1;
            unlink("pth");
            bind(usfd,(struct sockaddr*)&addr,sizeof(addr));
        }

        int n = recvfrom(usfd,a,sizeof(a),0,(struct sockaddr*)&addr,&llen);
        if (n>0)
            cout<<"recieved from p2:"<<a<<endl;
    }
    return 0;
}