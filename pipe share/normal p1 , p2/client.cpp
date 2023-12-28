#include<bits/stdc++.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include<arpa/inet.h>
#include<sys/un.h>
#include<csignal>
using namespace std;

int main ()
{
    int sfd = socket(AF_INET , SOCK_STREAM , 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(3000);

    int connectt = connect(sfd , (struct sockaddr*)&addr , sizeof(addr));
    if(connectt == -1)
    {
        perror("connect error");
        exit(1);
    }

    while (1)
    {
        string str;
        cin>>str;
        const char * buf = str.c_str();
        send(sfd , buf , strlen(buf),0);
        cout<<"sent"<<endl;
    }
}