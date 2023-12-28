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
#include <sys/shm.h>
using namespace std;

int main ()
{
   // send something to first.cpp to verify ip header , using raw sockets 
    int rsfd = socket(AF_INET,SOCK_RAW,11);
    if(rsfd<0)
    {
        cout<<"Socket Error\n";
        return 1;
    }
    
	perror("socket");
	
    struct sockaddr_in client;
	client.sin_family=AF_INET;
	client.sin_addr.s_addr=inet_addr("127.0.0.1");

    while (1)
    {
        string str;
        cin>>str;
        const char * buff = str.c_str();

        unsigned int client_len=sizeof(client);
        cout<<"sending"<<endl;
        sendto(rsfd,buff,strlen(buff)+1,0,(struct sockaddr*)&client,sizeof(client));
        perror("send");
    }
}