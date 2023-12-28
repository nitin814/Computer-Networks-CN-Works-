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
    mkfifo("talks" , 0666);

    int wfd = open("talks" , O_WRONLY);

    while (1)
    {
        string s;
        cin>>s;
        write(wfd , s.c_str() , s.length());
        cout<<"wrote "<<s<<endl;
    }
}