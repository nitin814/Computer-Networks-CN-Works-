#include <bits/stdc++.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;
#define ll long long

int main ()
{
    int f = mkfifo("nitt" , 0666);

    int wfd = open("nitt" , O_WRONLY);

    char buffer[] = {"what to do man"};

    write(wfd, buffer , sizeof(buffer));
}