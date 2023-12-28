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

int main() {
    // Create a raw socket for sending Ethernet frames
    int rawSocket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if (rawSocket == -1) {
        perror("Socket creation error");
        return -1;
    }

    // Destination MAC address (replace with the actual MAC address)
    unsigned char destMAC[] = {0xBC, 0x17, 0xB8, 0xCE, 0xA5, 0xF4};

    // Source MAC address (replace with the actual MAC address of your network interface)
    unsigned char sourceMAC[] = {0x40, 0x1C, 0x83, 0xA8, 0xF1, 0x79};

    // Ethernet frame structure
    struct ethhdr ethernetHeader;
    std::memcpy(ethernetHeader.h_dest, destMAC, ETH_ALEN);
    std::memcpy(ethernetHeader.h_source, sourceMAC, ETH_ALEN);
    ethernetHeader.h_proto = htons(ETH_P_IP);  // Example: IPv4 payload

    // Data to be sent (replace with your payload)
    const char* payload = "Hello, Ethernet!";

    // Prepare the complete frame
    char frame[sizeof(ethernetHeader) + strlen(payload)];
    std::memcpy(frame, &ethernetHeader, sizeof(ethernetHeader));
    std::memcpy(frame + sizeof(ethernetHeader), payload, strlen(payload));

    // Specify the network interface (replace "eth0" with your interface)
    struct sockaddr_ll sa;
    struct ifreq ifr;
    std::strcpy(ifr.ifr_name, "wlp0s20f3");
    std::memset(&sa, 0, sizeof(struct sockaddr_ll));
    sa.sll_ifindex = if_nametoindex(ifr.ifr_name);

    // Send the frame
    ssize_t bytesSent = sendto(rawSocket, frame, sizeof(frame), 0, (struct sockaddr*)&sa, sizeof(struct sockaddr_ll));

    if (bytesSent == -1) {
        perror("Packet send error");
    } else {
        std::cout << "Packet sent successfully\n";
    }

    // Close the raw socket
    close(rawSocket);

    return 0;
}

