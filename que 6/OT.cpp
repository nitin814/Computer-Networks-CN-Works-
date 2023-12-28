#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/un.h>
#include <poll.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <bits/stdc++.h>
#define BUF_LEN 65000

using namespace std;

int main ()
{
    int c = 0;
    c = fork();

    if (c == 0) 
    {
        const char *programName = "./I";
        char *args[] = { const_cast<char*>(programName), nullptr };
        execv(programName, args);
    } 

    int sfdless = socket(AF_INET , SOCK_DGRAM , 0);
    if (sfdless==-1)
    {
        cout<<"Error in creating socket"<<endl;
        return 0;
    }

    string rollnumber;
    cout<<"Enter your roll number"<<endl;
    cin>>rollnumber;
    
    struct sockaddr_in serveraddress;
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(9999);
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
   
    const char * buf = rollnumber.c_str();
    int n = sendto(sfdless , buf , strlen(buf)+1 , 0 , (struct sockaddr *)&serveraddress , sizeof(serveraddress));

    int cnt = 5;
    while (cnt--)
    {
        struct sockaddr_in clientaddress;
        char buffer[100]; 
        socklen_t client_len = sizeof(clientaddress);
        n = recvfrom(sfdless , &buffer , sizeof(buffer)-1 , 0 , (struct sockaddr *)&clientaddress , &client_len);
        buffer[n] = '\0';

        cout<<"Q: "<<5-cnt<<") "<<buffer<<endl;
    }
}