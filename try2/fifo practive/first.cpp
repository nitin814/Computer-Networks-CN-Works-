#include <bits/stdc++.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;
#define ll long long

int main ()
{
    int ifd = open ("./input.txt" , O_RDONLY);

    FILE* writeinto = popen("./p2" , "w");
    cout<<"connected"<<endl;
    char buffer[100];
    int n = 9;

    string str;
  
    while (n = read(ifd , buffer , 100))
    {   
        fwrite(buffer , 1,  100 , writeinto);
    }
    cout<<"done"<<endl;
}
