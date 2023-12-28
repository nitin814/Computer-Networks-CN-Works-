#include <bits/stdc++.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include<sys/wait.h>

using namespace std;

int main ()
{
    // int f = mkfifo("mind" , 0666);

    // int ifd = open ("mind" , O_RDONLY);

    // int ofd = open ("tempprog.cpp" , O_WRONLY);

    // char buffer[100]; int n;

    // while (n = read(ifd , buffer , sizeof(buffer)-1))
    // {
    //     buffer[n] = '\n';
    //     write (ofd , buffer , n+1);
    // }

    int f = mkfifo("mind" , 0666);
    cout<<f<<endl;
    ifstream ifd ("mind");
    
    ofstream ofd ("tempprog.cpp");
    
    string line;
    while (getline (ifd , line))
    {
        ofd<<line<<endl;
    }

    int compileResult = system("g++ tempprog.cpp -o tempprog");

    if (compileResult!=0)
    {
        cout<<"Compilation error , please check your code "<<endl;
        return 0;
    }

    int c = fork();

    if (c>0)
    {
        wait(NULL);
        
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
            if (linef1!=linef2)
            {   
                flag = 1;
                break;
            }
        }
        
        if (realout.eof())
            getline(tempcodeout , linef2);

        if (flag==0)
        {
            if (!realout.eof() || !tempcodeout.eof())
            {
                cout<<"mo"<<endl;
                flag = 1;
            }
        }

        int fifd = open ("finalmessage.txt" , O_WRONLY);

        const char *buffer;
        if (flag)
        {
            buffer = "Test cases failed ";
        }
        else
        {
            buffer = "Test cases passed ";
        }

        write(fifd , buffer , strlen(buffer));
    }
    else
    {
        int ifd = open("realin.txt" , O_RDONLY);
        int ofd = open("tempcodeout.txt" , O_WRONLY);

        dup2 (ifd , 0);
        dup2 (ofd , 1);

        char * args[] = {NULL};
        execv("./tempprog" , args);
        close(ifd);
        close(ofd);
    }
}