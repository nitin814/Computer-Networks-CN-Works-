#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;
int main() {
    const char* fifo2 = "/tmp/fifo2";
    
    // Open fifo2 for writing
    int fd2 = open(fifo2, O_WRONLY);
    
    if (fd2 == -1) {
        perror("open");
        return 1;
    }
    
     while (1)
    {
        string str;
        cin>>str;
        const char* message = str.c_str();
        
        // Write the message into fifo1
        if (write(fd2, message, strlen(message)) == -1) 
        {
            perror("write");
        }
    }
    
    close(fd2);
    
    return 0;
}
