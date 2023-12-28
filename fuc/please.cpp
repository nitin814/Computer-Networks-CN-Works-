#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/if_ether.h>
#include <net/if.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <linux/if_packet.h>

#define ETHERTYPE_ARP 0x0806
#define ARP_REQUEST 1
#define ETHERNET_HEADER_SIZE sizeof(struct ether_header)
#define ARP_HEADER_SIZE sizeof(struct ether_arp)

void die(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    // Create a raw socket
    int sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sockfd == -1) {
        die("Error creating socket");
    }

    // Set interface name (replace "lo" with your loopback interface)
    const char *interface = "lo";

    // Get interface index
    struct ifreq ifr;
    strncpy(ifr.ifr_name, interface, IFNAMSIZ);
    if (ioctl(sockfd, SIOCGIFINDEX, &ifr) == -1) {
        die("Error getting interface index");
    }
    
    int ifindex = ifr.ifr_ifindex;

    // Prepare the Ethernet frame
    struct ether_header eth_header;
    unsigned char dest_mac[6] = {0x40, 0x1C, 0x83, 0xA8, 0xF1, 0x79}; // Broadcast MAC for ARP request
    unsigned char src_mac[6] = {0x40, 0x1C, 0x83, 0xA8, 0xF1, 0x79};  // Replace with source MAC
    eth_header.ether_type = htons(ETHERTYPE_ARP);
    memcpy(eth_header.ether_dhost, dest_mac, ETH_ALEN);
    memcpy(eth_header.ether_shost, src_mac, ETH_ALEN);

    // Prepare the ARP header
    struct ether_arp arp_header;
    arp_header.arp_hrd = htons(ARPHRD_ETHER);
    arp_header.arp_pro = htons(ETHERTYPE_IP);
    arp_header.arp_hln = ETH_ALEN;
    arp_header.arp_pln = 4;
    arp_header.arp_op = htons(ARP_REQUEST);
    memcpy(arp_header.arp_sha, src_mac, ETH_ALEN);

    // Use inet_pton to convert source IP address
    if (inet_pton(AF_INET, "127.0.0.1", &arp_header.arp_spa) != 1) {
        fprintf(stderr, "Error converting source IP address\n");
        return EXIT_FAILURE;
    }

    memset(arp_header.arp_tha, 0, ETH_ALEN);

    // Use inet_pton to convert target IP address
    if (inet_pton(AF_INET, "127.0.0.1", &arp_header.arp_tpa) != 1) {
        fprintf(stderr, "Error converting target IP address\n");
        return EXIT_FAILURE;
    }

    // Construct the complete packet
    char packet[ETHERNET_HEADER_SIZE + ARP_HEADER_SIZE];
    memcpy(packet, &eth_header, ETHERNET_HEADER_SIZE);
    memcpy(packet + ETHERNET_HEADER_SIZE, &arp_header, ARP_HEADER_SIZE);

    // Send the packet to the loopback interface
    struct sockaddr_ll sa;
    memset(&sa, 0, sizeof(struct sockaddr_ll));
    sa.sll_family = AF_PACKET;
    sa.sll_ifindex = ifindex;
    sa.sll_protocol = htons(ETH_P_ALL);

    ssize_t bytes_sent = sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr *)&sa, sizeof(sa));
    if (bytes_sent == -1) {
        die("Error sending packet");
    }

    printf("Sent %zd bytes (ARP request) to the loopback interface\n", bytes_sent);

    // Close the socket
    close(sockfd);

    return 0;
}



