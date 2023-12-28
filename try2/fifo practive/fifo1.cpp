#include <bits/stdc++.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;
#define ll long long

int main ()
{
    int rdf = open("nitt" , O_RDONLY);

    char buffer[10];
    read(rdf , buffer , 10);
    cout<<buffer<<endl;
}