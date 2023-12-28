#include <pcap.h>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>

#define ETHERTYPE_ARP 0x0806
#define ARP_REQUEST 1
#define ARP_REPLY 2
#define ETHERNET_HEADER_SIZE sizeof(struct ether_header)
#define ARP_HEADER_SIZE sizeof(struct ether_arp)

void die(const char *msg) {
    perror(msg);
    std::exit(EXIT_FAILURE);
}

void send_arp_reply(const struct ether_arp *arp_request, const char *interface) {
    // Create a raw socket
    int sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sockfd == -1) {
        die("Error creating socket");
    }

    // Prepare the Ethernet frame
    struct ether_header eth_header;
    memcpy(eth_header.ether_dhost, arp_request->arp_sha, ETH_ALEN);
    unsigned char src_mac[6] = {0x40, 0x1C, 0x83, 0xA8, 0xF1, 0x79};
    memcpy(eth_header.ether_shost, src_mac, ETH_ALEN);
    eth_header.ether_type = htons(ETHERTYPE_ARP);
    
    // Prepare the ARP reply
    struct ether_arp arp_reply;
    arp_reply.arp_hrd = htons(ARPHRD_ETHER);
    arp_reply.arp_pro = htons(ETHERTYPE_IP);
    arp_reply.arp_hln = ETH_ALEN;
    arp_reply.arp_pln = 4;
    arp_reply.arp_op = htons(ARP_REPLY);
    memcpy(arp_reply.arp_sha, src_mac, ETH_ALEN);
    memcpy(arp_reply.arp_spa, arp_request->arp_tpa, 4);
    memcpy(arp_reply.arp_tha, arp_request->arp_sha, ETH_ALEN);
    memcpy(arp_reply.arp_tpa, arp_request->arp_spa, 4);

    // Construct the complete packet
    char packet[ETHERNET_HEADER_SIZE + ARP_HEADER_SIZE];
    memcpy(packet, &eth_header, ETHERNET_HEADER_SIZE);
    memcpy(packet + ETHERNET_HEADER_SIZE, &arp_reply, ARP_HEADER_SIZE);

    // Get interface index
    struct ifreq ifr;
    strncpy(ifr.ifr_name, interface, IFNAMSIZ);
    if (ioctl(sockfd, SIOCGIFINDEX, &ifr) == -1) {
        die("Error getting interface index");
    }
    int ifindex = ifr.ifr_ifindex;

    // Send the packet back to the interface
    struct sockaddr_ll sa;
    memset(&sa, 0, sizeof(struct sockaddr_ll));
    sa.sll_family = AF_PACKET;
    sa.sll_ifindex = ifindex;
    sa.sll_protocol = htons(ETH_P_ALL);

    ssize_t bytes_sent = sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr *)&sa, sizeof(sa));
    if (bytes_sent == -1) {
        die("Error sending ARP reply");
    }

    std::cout << "Sent " << bytes_sent << " bytes (ARP reply) to the interface" << std::endl;

    // Close the socket
    close(sockfd);
}

void packet_handler(unsigned char *user_data, const struct pcap_pkthdr *pkthdr, const unsigned char *packet) {
    struct ether_header *eth_header = (struct ether_header *)packet;

    if (ntohs(eth_header->ether_type) == ETHERTYPE_ARP) {
        struct ether_arp *arp_packet = (struct ether_arp *)(packet + ETHERNET_HEADER_SIZE);

        if (ntohs(arp_packet->arp_op) == ARP_REQUEST) {
            std::cout << "Received ARP request" << std::endl;

            // Extract source MAC address
            unsigned char source_mac[ETH_ALEN];
            memcpy(source_mac, eth_header->ether_shost, ETH_ALEN);

            // Extract source IP address
            struct in_addr source_ip;
            memcpy(&source_ip, arp_packet->arp_spa, 4);

            // Print source MAC and IP
            std::cout << "Source MAC Address: ";
            for (int i = 0; i < ETH_ALEN; ++i) {
                printf("%02x", source_mac[i]);
                if (i < ETH_ALEN - 1) {
                    std::cout << ":";
                }
            }
            std::cout << std::endl;

            char source_ip_str[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &source_ip, source_ip_str, sizeof(source_ip_str));
            std::cout << "Source IP Address: " << source_ip_str << std::endl;

            // Send ARP reply
            send_arp_reply(arp_packet, "wlp0s20f3"); // Replace "eth0" with your interface name
        }
    }
}


int main() {
    char errbuf[PCAP_ERRBUF_SIZE];

    // Open a live capture handle
    pcap_t *handle = pcap_open_live("wlp0s20f3", BUFSIZ, 1, 1000, errbuf); // Replace "eth0" with your interface name
    if (handle == nullptr) {
        die(errbuf);
    }

    // Set a filter to capture only ARP packets
    struct bpf_program fp;
    char filter_exp[] = "arp";
    if (pcap_compile(handle, &fp, filter_exp, 0, PCAP_NETMASK_UNKNOWN) == -1) {
        die(pcap_geterr(handle));
    }
    if (pcap_setfilter(handle, &fp) == -1) {
        die(pcap_geterr(handle));
    }

    // Start capturing packets
    pcap_loop(handle, 0, packet_handler, nullptr);

    // Close the handle (this code is never reached in this example)
    pcap_close(handle);

    return 0;
}
