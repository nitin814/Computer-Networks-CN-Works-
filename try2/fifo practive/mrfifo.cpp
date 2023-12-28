#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    const char *pipeName = "chat_pipe";


    // Open the pipe for writing
    int pipefd = open(pipeName, O_WRONLY);
    if (pipefd == -1) {
        std::cerr << "Writer: Failed to open pipe for writing." << std::endl;
        return 1;
    }

    const char *message = "Hello from Writer1!";
    write(pipefd, message, strlen(message) + 1);
    close(pipefd);

    return 0;
}
