#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>

using namespace std;

int main ()
{
    int sfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sfd == -1)
    {
        cout<<"Error in creating socket "<<endl;
        return 0;
    }

    struct sockaddr_in serveraddress;
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(3002);
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    int t = connect (sfd , (sockaddr *)&serveraddress , sizeof(serveraddress));
    if (t==-1)
    {
        cout<<"Error in connecting "<<endl;
        return 1;
    }

    struct pollfd pfd[2];
    pfd[0].fd = 0; pfd[0].events = POLLIN;
    pfd[1].fd = sfd; pfd[1].events = POLLIN;

    while (1)
    {
        int s = poll(pfd , 2,  1000);
        if (s<=0)
            continue;
        if (pfd[0].revents & POLLIN) 
        {
            string str;
            getline(cin , str);
            cout<<"sending : "<<str<<endl;
            const char * buf = str.c_str();
            send(sfd , buf , strlen(buf)+1 , 0);
            cout<<"sending done succesful"<<endl;
        }
        else if (pfd[1].revents & POLLIN)
        {
            // cout<<"activated"<<endl;
            char buffer[1000]; int n;
            n = recv(sfd , buffer , sizeof(buffer)-1 , 0);
            buffer[n] = '\0';
            // cout<<"New message  : "<<buffer<<endl;
            cout<<buffer<<endl;
        }
    }
    
}