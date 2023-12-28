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

vector<int> socketfds;
string sending = "";
map<int,int> mp;

void * func (void * args)
{
    int rsfd = socket(AF_INET , SOCK_DGRAM , 0);
    if (rsfd==-1)
    {
        cout<<"Error in creating socket"<<endl;
        return NULL;
    }
    cout<<"yoyooyo from thread"<<endl;
    
    struct sockaddr_in serveraddress;
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(9999);
    serveraddress.sin_addr.s_addr = INADDR_ANY;

    int binderror = bind (rsfd , (struct sockaddr *)&serveraddress , sizeof(serveraddress));
    if (binderror == -1)
    {
        cout<<"Error in binding "<<endl;
        return NULL;
    }

    while (1)
    {
        struct sockaddr_in clientaddress;
        char buffer[100]; 
        socklen_t client_len = sizeof(clientaddress);
        int n = recvfrom(rsfd , &buffer , sizeof(buffer)-1 , 0 , (struct sockaddr *)&clientaddress , &client_len);
        buffer[n] = '\0';

        cout<<"reading successful !"<<endl;
        const char * buf = sending.c_str();
        sendto(rsfd , buf , strlen(buf)+1 , 0 , (struct sockaddr *)&clientaddress , sizeof(clientaddress));
        cout<<"sending successful"<<endl;
    }
}

int main ()
{
    pthread_t listens;
    pthread_create(&listens , NULL , func , NULL);

    struct pollfd pfd[10];
    // pfd[0].fd = rsfd; pfd[1].events = POLLIN;
    pfd[1].fd = 0; pfd[1].events = POLLIN;
    int cnt = 2;

    while (1)
    {
        int s = poll(pfd , cnt , 1000);
        
        if (s<=0)
            continue;
        sleep(1);
        cout<<s<<endl;
        for (int i=0;i<cnt;i++)
        {
            if (i==0 && (pfd[0].revents & POLLIN))
            {
            
            }
            else if (i==1 && (pfd[1].revents & POLLIN))
            {
                cout<<"came in here !"<<endl;
                int s = 0;
                string str1,str2;
                cin>>str1>>str2;    // port , path ...

                cout<<str1<<" "<<str2<<endl;
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
                serveraddress.sin_port = htons(stoi(str1));
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

                pfd[cnt].fd = sfd; pfd[cnt].events = POLLIN;
                cnt++;
                sending += str1;
                sending += " , ";
                int c = 0;
                c = fork();
                mp[sfd] = c;
                if (c==0)
                {
                    const char * exefile = str2.c_str();
                    cout<<"yoyoy"<<endl;
                    dup2(sfd , 1);
                    execv(exefile , NULL);
                }
                else
                {
                    sleep(2);
                }
            }
            else if (i!=0 && i!=1)
            {
                if (pfd[i].revents & POLLIN)
                {
                   // pfd[i].revents = !(pfd[i].revents & POLLIN);
                    cout<<"got kicked"<<endl;
                    kill (mp[pfd[i].fd] , SIGUSR1);
                    sleep(2);
                }
            }
        }
    }
}