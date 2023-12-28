#include <bits/stdc++.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;

int main()
{
    const char* fifo1 = "/tmp/fifo1";
    const char* fifo2 = "/tmp/fifo2";
    
    // Create two named pipes (FIFOs)
    mkfifo(fifo1, 0666);
    mkfifo(fifo2, 0666);
    
    int fd1 = open(fifo1, O_RDONLY | O_NONBLOCK);
    int fd2 = open(fifo2, O_RDONLY | O_NONBLOCK);
    
    if (fd1 == -1 || fd2 == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(fd1, &read_fds);
    FD_SET(fd2, &read_fds);
    
    int max_fd = max(fd1, fd2) + 1;
    struct timeval timeout;
    timeout.tv_sec = 6;
    timeout.tv_usec = 0;

    while (true) {
        fd_set temp_fds = read_fds;
        int activity = select(max_fd, &temp_fds, nullptr, nullptr, &timeout);
        
        if (activity == -1) {
            perror("select");
            exit(EXIT_FAILURE);
        }
        if (activity == 0) {
            cout << "Timeout done !!" << endl;
            continue;
        }
        
        if (FD_ISSET(fd1, &temp_fds)) {
            // fd1 is ready for reading
            char buffer[256];
            ssize_t bytes_read = read(fd1, buffer, sizeof(buffer));
            if (bytes_read > 0) {
                buffer[bytes_read] = '\0';
                cout << "Received from fifo1: " << buffer << endl;
            }
        }
        
        if (FD_ISSET(fd2, &temp_fds)) {
            // fd2 is ready for reading
            char buffer[256];
            ssize_t bytes_read = read(fd2, buffer, sizeof(buffer));
            if (bytes_read > 0) {
                buffer[bytes_read] = '\0';
                cout << "Received from fifo2: " << buffer << endl;
            }
        }
    }
    
    close(fd1);
    close(fd2);
    
    // Clean up the FIFOs
    unlink(fifo1);
    unlink(fifo2);
    
    return 0;
}
