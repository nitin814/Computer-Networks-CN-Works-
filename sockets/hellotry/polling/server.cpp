#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>

using namespace std;

int main ()
{
    struct pollfd pfds[3];

    for (int i=0;i<3;i++)
    {
        int sfd = socket(AF_INET , SOCK_STREAM , 0);
        if (sfd == -1)
        {
            cout<<"Error in creating socket "<<endl;
            return 0;
        }

        struct sockaddr_in serveraddress;
        serveraddress.sin_family = AF_INET;
        serveraddress.sin_addr.s_addr = INADDR_ANY;
        serveraddress.sin_port = htons(3000+i);

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

        pfds[i].fd = sfd;
        pfds[i].events = POLLIN;
    }
    
    struct sockaddr_in clientaddress;
    socklen_t len = sizeof(clientaddress);

    while (1)
    {
        int s = poll (pfds , 3 , 1000);

        if (s<=0)
            continue;
            
        for (int i=0;i<3;i++)
        {   
            if (pfds[i].revents & POLLIN)
            {   
                cout<<"yoy"<<endl;
                int newsfd = accept (pfds[i].fd , (sockaddr *)&clientaddress , &len);
                if (newsfd==-1)
                {
                    cout<<"connection faliure"<<endl;
                    return 0;
                }

                int c = 0;
                c = fork();

                if (c==0)
                {
                    close(pfds[i].fd);
                    sleep(0.5);

                    char buffer[100]; int n;
                    n = recv(newsfd , buffer , sizeof(buffer-1) , 0);
                    buffer[n] = '\0'; string word (buffer);
                
                    if (word=="add")
                    {
                        char *args[]={"./add",NULL};
                        execvp(args[0] , args);
                    }
                    else if (word=="sub")
                    {
                        char *args[]={"./sub",NULL};
                        execvp(args[0] , args);
                    }
                }
                else
                {
                    close(newsfd);
                    sleep(0.5);
                }
            }
        }
    }
    
}