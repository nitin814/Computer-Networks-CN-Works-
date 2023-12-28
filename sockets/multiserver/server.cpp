#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sstream>

using namespace std;

int main ()
{
    cout<<"yoy"<<endl;
    int sfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sfd==-1)
    {
        cout<<"Error in creating socket"<<endl;
        return 1;
    }

    struct sockaddr_in serveraddress;
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(55553);
    serveraddress.sin_addr.s_addr = INADDR_ANY;

    cout<<"three"<<endl;

    int binderror = bind (sfd , (struct sockaddr *)&serveraddress , sizeof(serveraddress));

    if (binderror == -1)
    {
        cout<<"Error in binding "<<endl;
        return 0;
    }
    cout<<"one"<<endl;

    int listenerror = listen(sfd , 10);

    if (listenerror == -1)
    {
        cout<<"Listening error is found "<<endl;
        return 0;
    }

    cout<<"two"<<endl;
    
    while (1)
    {
        struct sockaddr * clientaddr;
        socklen_t len = sizeof(clientaddr);

        int newfd = accept(sfd , (struct sockaddr*)&clientaddr , &len);

        cout<<newfd<<endl;
        if (newfd==-1)
        {
            cout<<"not"<<endl;
            continue;
        }
        
        int c = 0;
        c = fork();

        if (c>0)   
        {
            close(newfd);
            sleep(0.5);
        }
        else  
        {   
            close(sfd);
            sleep(0.5);

            char buffer[1024]; int n;
            while ((n = recv(newfd , buffer , sizeof(buffer)-1 , 0))>0)
            {
                buffer[n] = '\0';
                string word (buffer);

                cout<<word<<endl;
                
                dup2(newfd , 1);
                dup2(newfd , 0);

                if (word=="add")
                {   
                    char *args[]={"./add",NULL};
                    execvp(args[0],args);
                }
                else if (word=="subtract")
                {
                    char *args[]={"./subtract",NULL};
                    execvp(args[0],args);
                }
                else if (word=="multiply")
                {
                    char *args[]={"./multiply",NULL};
                    execvp(args[0],args);
                }
            }
        }
    }
}