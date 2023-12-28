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
    serveraddress.sin_port = htons(3001);

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

                pfd[cnt].fd = newsfd; pfd[cnt].events = POLLIN;
                cnt++;
            }
            else
            {
                if (pfd[i].revents & POLLIN)
                {
                    char buffer [100];
                    recv(pfd[i].fd , buffer , sizeof(buffer)+1 , 0);

                    if (!mp.count(i))
                    {
                        cout<<"Got new client "<<endl;
                        int sfd = socket(AF_INET , SOCK_STREAM , 0);
                        if (sfd == -1)
                        {
                            cout<<"Error in creating socket "<<endl;
                            return 0;
                        }

                        struct sockaddr_in serveraddress;
                        string port (buffer);
                        int t = stoi(port);
                        cout<<"GOint to port : "<<t<<" to connect"<<endl;
                        serveraddress.sin_family = AF_INET;
                        serveraddress.sin_port = htons(t);
                        serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
                        
                        t = connect (sfd , (sockaddr *)&serveraddress , sizeof(serveraddress));
                        if (t==-1)
                        {
                            cout<<"Error in connecting "<<endl;
                            return 1;
                        }

                        mp[i] = sfd;
                    }
                    else
                    {
                        send(mp[i] , buffer , strlen(buffer)+1 , 0);
                        cout<<"Sending message to server from proxy server"<<endl;

                        char buf[100];
                        recv(mp[i] , buf , sizeof(buf) , 0);
                        cout<<"got the text "<<buf<<endl;

                        int n = send(pfd[i].fd , buf , strlen(buf)+1 , 0);
                        cout<<"sended the text"<<endl;

                        sleep(3);
                        cout<<n<<endl;
                    }
                }
            }
        }
    }
}