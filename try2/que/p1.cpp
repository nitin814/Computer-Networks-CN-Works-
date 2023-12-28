#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>

const char* socket_path = "/tmp/my_socket";

int main() {
    int sockfd, client_fd;
    struct sockaddr_un server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Create a Unix domain socket
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return 1;
    }
	unlink(socket_path);
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, socket_path, sizeof(server_addr.sun_path) - 1);

    // Bind the socket to a file path
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        return 1;
    }

    // Listen for incoming connections
    if (listen(sockfd, 5) == -1) {
        perror("listen");
        return 1;
    }

    std::cout << "Server is waiting for connections..." << std::endl;

    
        // Accept incoming connection
        client_fd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd == -1) {
            perror("accept");
            
        }
	
	 std::string message = "This is message ";
            struct iovec iov[1];
            iov[0].iov_base = (void*)message.c_str();
            iov[0].iov_len = message.length();

            // Create the message header
            struct msghdr msg = {0};
            msg.msg_iov = iov;
            msg.msg_iovlen = 1;
            
        for (int i = 1; i <= 2; ++i) {
  
            // Send the message
            if (sendmsg(client_fd, &msg, 0) == -1) {
                perror("sendmsg");
                close(client_fd);
              
            }
		sleep(3);
            std::cout << "Sent: " << message << std::endl;
        }
	
	while(1)
	{}
        close(client_fd);
    

    return 0;
}

