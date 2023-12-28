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

using namespace std;

int main ()
{
    struct pollfd pfd[10];
    int cnt = 1;
    int sfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sfd == -1)
    {
        cout<<"Error in creating socket "<<endl;
        return 0;
    }

    int reuse = 1;
    map<int,int> mp;
   
    struct sockaddr_in serveraddress;
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_addr.s_addr = INADDR_ANY;
    serveraddress.sin_port = htons(8002);

    int binderror = bind (sfd , (struct sockaddr*)&serveraddress , sizeof(serveraddress));
    if (binderror==-1)
    {
        cout<<"Error in binding "<<endl;
        return 0;
    } 
    if (listen (sfd , 10)==-1)
    {
        cout<<"error in listening "<<endl;
        return 0;
    }

    pfd[0].fd = sfd; pfd[0].events = POLLIN;

    while (1)
    {
        int s = poll(pfd , cnt , 1000);
        if (s<=0)
            continue;

        for (int i=0;i<cnt;i++)
        {
            if (i==0 && (pfd[0].revents & POLLIN))
            {
                struct sockaddr_in clientaddress;
                socklen_t len = sizeof(clientaddress);

                int newsfd = accept (pfd[0].fd , (sockaddr *)&clientaddress , &len);
                if (newsfd==-1)
                {
                    cout<<"connection faliure"<<endl;
                    return 0;
                }
                cout<<"Accepting the client now "<<endl;
                pfd[cnt].fd = newsfd; pfd[cnt].events = POLLIN;
                cnt++;
            }
            else
            {
                if (pfd[i].revents & POLLIN)
                {
                    char buffer[100];
                    recv(pfd[i].fd , buffer , sizeof(buffer)+1 , 0);
                    string text (buffer);
                    reverse(text.begin() , text.end());
                    cout<<"got the text : "<<text<<endl;

                    const char * buf = text.c_str();
                    send(pfd[i].fd , buf , strlen(buf)+1 , 0);
                    cout<<"Sended the text "<<text<<endl;
                }
            }
        }
    }
}