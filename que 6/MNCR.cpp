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
    int sfdless = socket(AF_INET , SOCK_DGRAM , 0);
    if (sfdless==-1)
    {
        cout<<"Error in creating socket"<<endl;
        return 0;
    }

    struct sockaddr_in clientaddress;
    char buffer[100]; 
    socklen_t client_len = sizeof(clientaddress);
    // recieve the roll number (name) from OT and use the clientaddess(OT) for later communications 
    int n = recvfrom(sfdless , &buffer , sizeof(buffer)-1 , 0 , (struct sockaddr *)&clientaddress , &client_len);
    buffer[n] = '\0';

    struct sockaddr_in clientaddress2;
    buffer[100]; 
    socklen_t client_len2 = sizeof(clientaddress2);

    int cnt = 10;
    // considering 10 questions 
    while (cnt--)
    {
        string question;
        cin>>question;
        const char * buf = question.c_str();
        int n = sendto(sfdless , buf , strlen(buf)+1 , 0 , (struct sockaddr *)&clientaddress , sizeof(clientaddress));
        cout<<"Sended question to OT "<<endl;

        n = recvfrom(sfdless , &buffer , sizeof(buffer)-1 , 0 , (struct sockaddr *)&clientaddress2 , &client_len2);
        buffer[n] = '\0';
        cout<<"Answer recievedd "<<endl;
    }
    
    n = recvfrom(sfdless , &buffer , sizeof(buffer)-1 , 0 , (struct sockaddr *)&clientaddress2 , &client_len2);
    buffer[n] = '\0';
    string res (buffer);
    // send message to OT on the basis of cnt recieved from I 
    if (res=="0")
    {
        string ress = "SUCCESS";
        const char * buf = ress.c_str();
        int n = sendto(sfdless , buf , strlen(buf)+1 , 0 , (struct sockaddr *)&clientaddress , sizeof(clientaddress));
    }
    else
    {
        string ress = "Not OWN";
        const char * buf = ress.c_str();
        int n = sendto(sfdless , buf , strlen(buf)+1 , 0 , (struct sockaddr *)&clientaddress , sizeof(clientaddress));
    }
}