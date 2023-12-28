#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
using namespace std;



int main() 
{ 
    int ifd = open("input.txt", O_RDONLY);
    
    int ifddup = fcntl(ifd, F_DUPFD, 0); // value greater than or equal to 0;

    int ofd = open ("output.txt" , O_WRONLY);

    cout<<"Original FD: "<<ifd<<endl;
    cout<<"Duplicated FD: "<<ifddup<<endl;

    // int flags = fcntl(originalFd, F_GETFD, 0); 
    // flags |= FD_CLOEXEC; 
    // fcntl(originalFd, F_SETFD, flags);

    char buffer[1000];
    read (ifd , buffer , sizeof(buffer));

    write (ofd , buffer , strlen(buffer));
    write (ofd , buffer , strlen(buffer));

    close(ifd);
    close(ifddup);

    return 0;
}