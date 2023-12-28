#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;
int main() 
{
    const char* fifo1 = "/tmp/fifo1";
    
    // Open fifo1 for writing
    int fd1 = open(fifo1, O_WRONLY);
    
    if (fd1 == -1) {
        perror("open");
        return 1;
    }
    
    while (1)
    {
        string str;
        cin>>str;
        const char* message = str.c_str();
        
        // Write the message into fifo1
        if (write(fd1, message, strlen(message)) == -1) 
        {
            perror("write");
        }
    }
    
    close(fd1);
    
    return 0;
}
