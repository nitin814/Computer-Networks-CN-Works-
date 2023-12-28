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
    mkfifo("fd1" , 0666);
  
    int ofd = open("fd1" , O_WRONLY);    

    int ifd = open("prog1.cpp" , O_RDONLY);

    int n; char buffer[100];
    while (n = read(ifd , buffer , sizeof(buffer)-1))
    {
        buffer[n] = '\0';
        write(ofd , buffer , n);
    }
    cout<<"done putting data into the temp file"<<endl;
    // wait(NULL);
    close(ofd);

    
    mkfifo("fd1a" , 0666);
    int ofdd = open("fd1a" , O_RDONLY); 
    // int n;
    char buffer1[100];
    n = read(ofdd , buffer1 , sizeof(buffer1)-1);
    buffer1[n] = '\0';

    cout<<"Final Verdict : ";
    cout<<buffer1<<endl;
    close(ofdd);
}