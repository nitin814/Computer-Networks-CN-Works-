#include <bits/stdc++.h>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include <netinet/ether.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <sys/ioctl.h>


int main() {
    // Create a raw socket for receiving Ethernet frames
    int rawSocket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if (rawSocket == -1) {
        perror("Socket creation error");
        return -1;
    }

    // Replace "eth0" with the name of your network interface
    const char* interface = "eth0";

    // Set up sockaddr struct for binding to a specific interface
    struct sockaddr_ll sa;
    struct ifreq ifr;

    std::memset(&ifr, 0, sizeof(struct ifreq));
    std::strncpy(ifr.ifr_name, interface, IFNAMSIZ - 1);

    if (ioctl(rawSocket, SIOCGIFINDEX, &ifr) == -1) {
        perror("ioctl error");
        close(rawSocket);
        return -1;
    }

    sa.sll_family = AF_PACKET;
    sa.sll_protocol = htons(ETH_P_ALL);
    sa.sll_ifindex = ifr.ifr_ifindex;

    // Bind the socket to the specific network interface
    if (bind(rawSocket, (struct sockaddr*)&sa, sizeof(struct sockaddr_ll)) == -1) {
        perror("Bind error");
        close(rawSocket);
        return -1;
    }

    // Start receiving and processing Ethernet frames
    while (true) {
        char buffer[ETH_FRAME_LEN];
        ssize_t bytesRead = recv(rawSocket, buffer, sizeof(buffer), 0);

        if (bytesRead == -1) {
            perror("Receive error");
            close(rawSocket);
            return -1;
        }

        std::cout << "Received a frame of size " << bytesRead << " bytes\n";
        // Add your frame processing logic here
    }

    // Close the raw socket (this won't be reached in the infinite loop)
    close(rawSocket);

    return 0;
}

