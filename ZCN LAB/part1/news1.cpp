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
#include <sstream>
using namespace std;

int main ()
{
    int sfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sfd==-1)
    {
        cout<<"Error in creating socket"<<endl;
        return 1;
    }

    int reuse = 1;
    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
    perror("setsockopt(SO_REUSEADDR) failed");
    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEPORT, (const char*)&reuse, sizeof(reuse)) < 0) 
    perror("setsockopt(SO_REUSEPORT) failed");

    struct sockaddr_in serveraddress;
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(3000);
    serveraddress.sin_addr.s_addr = INADDR_ANY;

    int binderror = bind (sfd , (struct sockaddr *)&serveraddress , sizeof(serveraddress));
    if (binderror == -1)
    {
        cout<<"Error in binding "<<endl;
        return 0;
    }
  
    int listenerror = listen(sfd , 10);
    if (listenerror == -1)
    {
        cout<<"Listening error is found "<<endl;
        return 0;
    }

    while (1)
    {   
        struct sockaddr_in clientaddr;
        socklen_t len = sizeof(clientaddr);
        int newfd = accept(sfd , (struct sockaddr*)&clientaddr , &len);
        if (newfd == -1)
        {
            cout<<"not"<<endl;
            continue;
        }

        char buffer[100];
        int n = recv(newfd , buffer , sizeof(buffer)-1 , 0);

        string str (buffer);
        stringstream s (str);
        string word;

        s>>word;
        if (word[0]>='0' && word[0]<='9')
        {
            int sfd = socket(AF_INET , SOCK_STREAM , 0);
            if (sfd==-1)
            {
                cout<<"Error in creating socket"<<endl;
                return 1;
            }

            struct sockaddr_in serveraddress;
            serveraddress.sin_family = AF_INET;
            serveraddress.sin_port = htons(stoi(word));
            serveraddress.sin_addr.s_addr = inet_addr("127.0.0.0");

            int t = connect (sfd , (sockaddr *)&serveraddress , sizeof(serveraddress));
            if (t==-1)
            {
                cout<<"Error in connecting "<<endl;
                return 1;
            }

            string remaining = str.substr(word.length()+1);
            const char * buf = remaining.c_str();

            cout<<"sending it to the live server "<<endl;
            send(sfd , buf , strlen(buf)+1 , 0);
            cout<<"sended to live server"<<endl;
        }
        else
        {
            cout<<"read the news by news: "<<buffer<<endl;
        }
        close(newfd);
    }
}