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
    struct pollfd pfd[3];
    
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

        pfd[i].fd = sfd; pfd[i].events = POLLIN;
        cout<<"sfd's are : "<<sfd<<endl;
    }

    while (1)
    {
        int s = poll(pfd , 3 , 1000);
        if (s<=0)
            continue;
        
        for (int i=0;i<3;i++)
        {
            if (pfd[i].revents & POLLIN)
            {   
                struct sockaddr_in clientaddress;
                socklen_t len = sizeof(clientaddress);

                int newsfd = accept (pfd[i].fd , (sockaddr *)&clientaddress , &len);
                if (newsfd==-1)
                {
                    cout<<"connection faliure"<<endl;
                    return 0;
                }

                int c = fork();
                if (c>0)
                {
                    wait(NULL);
                    ifstream realout ("realout.txt");
                    ifstream tempcodeout ("tempout.txt");
                    cout<<"shit"<<endl;
                    if (!realout || !tempcodeout)
                    {
                        cout<<"Server error , Try again later "<<endl;
                        return -1;
                    }

                    string linef1,linef2; 
                    bool flag = 0;

                    while (getline(realout , linef1) && getline(tempcodeout , linef2))
                    {   
                        cout<<linef1<<" "<<linef2<<endl;
                        if (linef1!=linef2)
                        {   
                            flag = 1;
                        }
                    }
                    
                    if (realout.eof())
                        getline(tempcodeout , linef2);  
                    if (!tempcodeout.eof())
                        cout<<"fuck"<<endl;
                    if (flag==0)
                    {
                        if (!realout.eof() || !tempcodeout.eof())
                        {
                            cout<<"mo"<<endl;
                            flag = 1;
                        }
                    }
                    string f;
                    if (flag)
                        f = "fail";
                    else
                        f = "pass";
                
                    const char * bx = f.c_str();

                    send(newsfd , bx , strlen(bx) , 0);
                    cout<<"donees"<<endl;
                }
                else
                {
                    int ofd = open("temp.cpp" , O_WRONLY);
                    char buffer[1000]; int n;
                    while (n = (recv(newsfd , buffer , sizeof(buffer)-1 , 0))>0)
                    {
                        cout<<buffer<<endl;
                        write(ofd , buffer , strlen(buffer));
                    }

                    cout<<"came out"<<endl;

                    int compileResult = system("g++ temp.cpp -o temp");
                    if (compileResult!=0)
                    {
                        cout<<"Compilation error , please check your code "<<endl;
                        return 0;
                    }
                    cout<<"done compiling "<<endl;

                    close(pfd[i].fd);
                    close(newsfd);
                    int wfd = open("tempout.txt" , O_WRONLY);
                    int rfd = open("tempin.txt" , O_RDONLY);
                    dup2(rfd , 0);
                    dup2(wfd , 1);

                    char * args[] = {NULL};
                    execv("./temp" , args);
                }
            }
        }
    }
}