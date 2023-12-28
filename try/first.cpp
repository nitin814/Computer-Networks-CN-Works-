#include<bits/stdc++.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>

using namespace std;

int main ()
{
    int fd = fileno(popen("./third" , "w"));
    int fd2 = fileno(popen("./second" , "w"));

    string str;
    getline(cin , str);

    const char * buff = str.c_str();
    write (fd , buff , strlen(buff)+1);
    write (fd2 , buff , strlen(buff)+1);

    cout<<"Done doing the everything "<<endl;
}