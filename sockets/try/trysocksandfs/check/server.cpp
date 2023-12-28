#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>

// why pointer of client and so much sending after stop. 
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
    
    struct sockaddr_in newaddress;socklen_t len;
    getsockname(sfd , (struct sockaddr *)&newaddress , &len);
    
    cout<<"I am sfd , my ip address is "<<inet_ntoa(newaddress.sin_addr)<<"  and my port is "<<ntohs(newaddress.sin_port)<<endl;
    
    struct pollfd pfd[10];
    pfd[0].fd = sfd; pfd[0].events = POLLIN;
    int lastp = 0;

    while (1)
    {
        int s = poll (pfd , lastp+1 , 1000);
        if (s<=0)
            continue;
            
        for (int i=0;i<=lastp;i++)
        {   
            if (i==0 && (pfd[0].revents & POLLIN))
            {
                struct sockaddr_in clientaddress;
                socklen_t len = sizeof(clientaddress);

                cout<<"yoy"<<endl;
                int newsfd = accept (pfd[0].fd , (sockaddr *)&clientaddress , &len);
                if (newsfd==-1)
                {
                    cout<<"connection faliure"<<endl;
                    return 0;
                }

                // struct sockaddr_in newaddress;socklen_t len;
    getsockname(sfd , (struct sockaddr *)&newaddress , &len);
    
    cout<<"I am sfd , my ip address is "<<inet_ntoa(newaddress.sin_addr)<<"  and my port is "<<ntohs(newaddress.sin_port)<<endl;
    

                getpeername(newsfd , (struct sockaddr *)&newaddress , &len);

                cout<<"I am newsfd , my ip address is "<<inet_ntoa(newaddress.sin_addr)<<"  and my port is "<<ntohs(newaddress.sin_port)<<endl;
                
                lastp++;
                pfd[lastp].fd = newsfd;
                pfd[lastp].events = POLLIN;
            }
            else if (pfd[i].revents & POLLIN)
            {
                char buffer[1000]; int n;
                n = recv(pfd[i].fd , buffer , sizeof(buffer) , 0);
                buffer[n] = '\0';
    
                for (int j = 1;j<=lastp;j++)
                {   
                    if (j!=i)
                        send(pfd[j].fd , buffer , strlen(buffer)+1 , 0);
                }
            }
        }
    }
}
    
