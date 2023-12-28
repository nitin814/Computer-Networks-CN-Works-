#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

using namespace std;
int main() 
{
    // Create a raw socket
    int raw_socket = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);

    if (raw_socket == -1) {
        perror("Failed to create raw socket");
        return 1;
    }

    // Enable IP_HDRINCL option to include your custom IP header
    int enable = 1;
    if (setsockopt(raw_socket, IPPROTO_IP, IP_HDRINCL, &enable, sizeof(enable)) < 0) {
        perror("Failed to set IP_HDRINCL option");
        return 1;
    }

    // Destination IP address
    const char* dest_ip = "127.0.0.1"; // Replace with the desired destination IP
    const char* source_ip = "62.61.60.11"; // Replace with the desired source IP

    // Set up the IP header
    struct iphdr ip_header;
    ip_header.version = 4;
    ip_header.ihl = 5;
    ip_header.tos = 0;
    ip_header.tot_len = htons(sizeof(struct iphdr)); // Total length is just the IP header
    ip_header.id = htons(54321); // You can choose any suitable value
    ip_header.frag_off = 0;
    ip_header.ttl = 64;
    ip_header.protocol = IPPROTO_TCP; // Assuming TCP
    ip_header.check = 0; // Checksum can be calculated but is optional
    ip_header.saddr = inet_addr(source_ip); // Source IP
    ip_header.daddr = inet_addr(dest_ip); // Destination IP

    // Combine the IP header into a single packet
    char packet[65536];
    std::memcpy(packet, &ip_header, sizeof(struct iphdr));

    // Specify the destination sockaddr
    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr.s_addr = inet_addr(dest_ip);

    // Send the packet
    if (sendto(raw_socket, packet, sizeof(struct iphdr), 0,
               (struct sockaddr*)&dest_addr, sizeof(dest_addr)) == -1) {
        perror("Failed to send packet");
        return 1;
    }

    cout<<"sended "<<endl;
    close(raw_socket);
    return 0;
}
