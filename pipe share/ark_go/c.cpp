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
    struct sockaddr_in serv_addr;
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.5");
    serv_addr.sin_port=htons(8002);
    
    connect(sfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
    cout<<"connected"<<endl;
    while(1)
    {
        char buf[100];
        cout<<"enter"<<endl;
        cin>>buf;
        send(sfd,buf,sizeof(buf),0);
    }
    return 0;
}