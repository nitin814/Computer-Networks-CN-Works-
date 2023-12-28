 #include <iostream>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    const char *pipeName = "chat_pipe";

    mkfifo(pipeName, 0666);

    // Open the pipe for reading
    int pipefd = open(pipeName, O_RDONLY);
    
    while (1)
    {
        if (pipefd == -1) {
        std::cerr << "Reader: Failed to open pipe for reading." << std::endl;
        return 1;
        }

        char buffer[1024];
        ssize_t bytesRead = read(pipefd, buffer, sizeof(buffer));
        if (bytesRead > 0) {
            std::cout << "Reader 1: Received message: " << buffer << std::endl;
        }
    }

    close(pipefd);

    return 0;
}

