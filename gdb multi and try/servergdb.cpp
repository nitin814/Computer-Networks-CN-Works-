#include <bits/stdc++.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <sys/wait.h>
#include <poll.h>

using namespace std;

int main ()
{
    // mkfifo("fd1" , 0666); mkfifo("fd2" , 0666); mkfifo("fd3" , 0666);
    // mkfifo("fd1a" , 0666); mkfifo("fd2a" , 0666);mkfifo("fd3a" , 0666);    

    struct pollfd pfd [3]; 
    int fd1 = open("fd1" , O_RDONLY);
    int fd2 = open("fd2" , O_RDONLY);
    int fd3 = open("fd3" , O_RDONLY);
    
    pfd[0].fd = fd1; pfd[0].events = POLLIN;
    pfd[1].fd = fd2; pfd[1].events = POLLIN;
    pfd[2].fd = fd3; pfd[2].events = POLLIN;

    cout<<"ok"<<endl;

    while (1)
    {
        int s = poll(pfd , 3 , 1000);
        
        if (s>0)
        {
            for (int i=0;i<3;i++)
            {
                if (pfd[i].revents & POLLIN)
                {   
                    ofstream file1("tempprog.cpp", ofstream::out | ofstream::trunc);
                    file1.close();

                    char buffer[2048];
                    int ofd = open("tempprog.cpp" , O_WRONLY);
                    int n;
                    while (n = read(pfd[i].fd , buffer , sizeof(buffer)-1))
                    {
                        buffer[n] = '\0';
                        write(ofd , buffer , n);
                    }

                    cout<<"df";

                    int compileResult = system("g++ tempprog.cpp -o tempprog");
        
                    if (compileResult!=0)
                    {
                        cout<<"Compilation error , please check your code "<<endl;
                        return 0;
                    }

                    int c = fork();
                    cout<<"somewhta"<<endl;
                    
                    if (c>0)
                    {
                        wait(NULL);
                        
                        cout<<"Program "<<(i+1)<<" is executing"<<endl;

                        ofstream file("tempprog.cpp", ofstream::out | ofstream::trunc);
                        file.close();

                        ifstream realout ("realout.txt");
                        ifstream tempcodeout ("tempcodeout.txt");

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
                                // break;
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

                        cout<<"hohigaya"<<endl;

                        char offd[]={'f', 'd', char('1'+ i ), 'a' , '\0'};

                        int ofd=open(offd,O_WRONLY);
                        
                        if (flag)
                            write(ofd, "Test Cases Failed", 17);
                        else
                            write(ofd, "Test Cases Passed", 17);
                    }
                    else
                    {
                        int ifd = open("./realin.txt" , O_RDONLY);
                        int ofd = open("./tempcodeout.txt" , O_WRONLY);

                        dup2 (ifd , 0);
                        dup2 (ofd , 1);

                        char * args[] = {NULL};
                        execv("./tempprog" , args);
                        close(ifd);
                        close(ofd);
                    }
                }
            }
        }
    }
}