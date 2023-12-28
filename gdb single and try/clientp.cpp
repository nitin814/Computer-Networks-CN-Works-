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
    // int ifd = open ("prog.cpp" , O_RDONLY);
    
    ifstream ifd ("prog.cpp");
    ofstream ofd ("mind");
    // int ofd = open ("mind" , O_WRONLY);

    // char buffer[100]; int n;
    // while (n = read(ifd , buffer , sizeof(buffer)-1))
    // {
    //     buffer[n] = '\n';
    //     write (ofd , buffer , n+1);
    // }
    string line;
    while (getline(ifd , line))
        ofd << line<<endl;

    wait(NULL);

    int ifdd = open("finalmessage.txt" , O_RDONLY);
    char buffer[100];

    read(ifdd , buffer , 100);
    cout<<"Final Verdict : ";
    cout<<buffer<<endl;
}