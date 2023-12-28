#include <iostream>
#include <pcap.h>

void packet_handler(unsigned char *user_data, const struct pcap_pkthdr *pkthdr, const unsigned char *packet) 
{
    // Extract information from the packet
    // Print source and destination MAC addresses
    // std::cout << "Source MAC: ";
    // for (int i = 0; i < 6; ++i) {
    //     printf("%02x", packet[i]);
    //     if (i < 5) {
    //         std::cout << ":";
    //     }
    // }
    // std::cout << std::endl;

    // std::cout << "Destination MAC: ";
    // for (int i = 6; i < 12; ++i) {
    //     printf("%02x", packet[i]);
    //     if (i < 11) {
    //         std::cout << ":";
    //     }
    // }
    // std::cout << std::endl;

    // Print source and destination IP addresses
    std::cout << "Source IP: ";
    for (int i = 26; i < 30; ++i) {
        printf("%d", packet[i]);
        if (i < 29) {
            std::cout << ".";
        }
    }
    std::cout << std::endl;

    std::cout << "Destination IP: ";
    for (int i = 30; i < 34; ++i) {
        printf("%d", packet[i]);
        if (i < 33) {
            std::cout << ".";
        }
    }
    std::cout << std::endl;

    std::cout<<std::endl;
    // Additional packet analysis can be performed here
}

int main() 
{
    char errbuf[PCAP_ERRBUF_SIZE];

    // Open a live capture handle
    pcap_t *handle = pcap_open_live("wlp0s20f3", BUFSIZ, 1, 1000, errbuf); // Replace "eth0" with your interface name
    if (handle == nullptr) {
        std::cerr << "Error opening pcap handle: " << errbuf << std::endl;
        return 1;
    }

    // Set a filter to capture all packets
    struct bpf_program fp;
    char filter_exp[] = "ip"; // Capture only IP packets, modify as needed
    if (pcap_compile(handle, &fp, filter_exp, 0, PCAP_NETMASK_UNKNOWN) == -1) {
        std::cerr << "Error compiling filter expression: " << pcap_geterr(handle) << std::endl;
        return 1;
    }
    if (pcap_setfilter(handle, &fp) == -1) {
        std::cerr << "Error setting filter: " << pcap_geterr(handle) << std::endl;
        return 1;
    }

    // Start capturing packets
    pcap_loop(handle, 0, packet_handler, nullptr);

    // Close the handle (this code is never reached in this example)
    pcap_close(handle);

    return 0;
}
