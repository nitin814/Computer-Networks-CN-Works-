#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>

const char* socket_path = "/tmp/my_socket";

int main() {
    int sockfd;
    struct sockaddr_un server_addr;
    char buffer[1024];

    // Create a Unix domain socket
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return 1;
    }

    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, socket_path, sizeof(server_addr.sun_path) - 1);

    // Connect to the socket
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        return 1;
    }

   
        // Receive a message
        struct iovec iov[1];
        iov[0].iov_base = buffer;
        iov[0].iov_len = sizeof(buffer);

        struct msghdr msg = {0};
        msg.msg_iov = iov;
        msg.msg_iovlen = 1;

        ssize_t bytes_received = recvmsg(sockfd, &msg, 0);
        std::cout<<bytes_received<<std::endl;
        sleep(2);

        buffer[bytes_received] = '\0';
        std::cout << "Received: " << buffer << std::endl;
    

    // Close the socket
    close(sockfd);

    return 0;
}

