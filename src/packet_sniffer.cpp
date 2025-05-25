#include <stdio.h>              // Standard I/O
#include <stdlib.h>             // For malloc(), exit()
#include <string.h>             // For memset()
#include <unistd.h>             // For close()
#include <arpa/inet.h>          // For htons()
#include <sys/socket.h>         // For socket(), recvfrom()
#include <netinet/ip.h>         // For IP header
#include <netinet/udp.h>        // For UDP header
#include <netinet/tcp.h>        // For TCP header
#include <netinet/ip_icmp.h>    // For ICMP header
#include <netinet/if_ether.h>   // For Ethernet header
#include <time.h>               // For time()

#include "packet_sniffer.h"     // Custom header
#include "protocol_parser.h"    // Protocol parsing

#define BUFFER_SIZE 65536

void start_sniffing() {
    int raw_socket;
    struct sockaddr saddr;
    socklen_t saddr_len = sizeof(saddr);
    unsigned char *buffer = (unsigned char *)malloc(BUFFER_SIZE);

    // Open log file
    FILE *logfile = fopen("../logs/packet.log", "a");
    if (!logfile) {
        perror("Log File Error");
        exit(EXIT_FAILURE);
    }

    // Create raw socket
    raw_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (raw_socket < 0) {
        perror("Socket Error");
        exit(EXIT_FAILURE);
    }

    printf("?? Sniffing started... Press Ctrl+C to stop\n");

    while (1) {
        int data_size = recvfrom(raw_socket, buffer, BUFFER_SIZE, 0, &saddr, &saddr_len);
        if (data_size < 0) {
            perror("? Error receiving packet");
            break;
        }

        struct iphdr *ip_header = (struct iphdr *)(buffer + sizeof(struct ethhdr));

        struct in_addr src, dst;
        src.s_addr = ip_header->saddr;
        dst.s_addr = ip_header->daddr;

        const char *protocol = "OTHER";
        int src_port = -1, dst_port = -1;

        if (ip_header->protocol == IPPROTO_TCP) {
            struct tcphdr *tcp = (struct tcphdr *)(buffer + ip_header->ihl * 4 + sizeof(struct ethhdr));
            src_port = ntohs(tcp->source);
            dst_port = ntohs(tcp->dest);
            protocol = "TCP";
        } else if (ip_header->protocol == IPPROTO_UDP) {
            struct udphdr *udp = (struct udphdr *)(buffer + ip_header->ihl * 4 + sizeof(struct ethhdr));
            src_port = ntohs(udp->source);
            dst_port = ntohs(udp->dest);
            protocol = "UDP";
        } else if (ip_header->protocol == IPPROTO_ICMP) {
            protocol = "ICMP";
        }

        // Get current time
        time_t now = time(NULL);
        struct tm *t = localtime(&now);

        // Log the details with time only
        fprintf(logfile, "[%02d:%02d:%02d] Protocol: %s | Source: %s:%d -> Dest: %s:%d | Packet Size: %d bytes\n",
                t->tm_hour, t->tm_min, t->tm_sec,
                protocol,
                inet_ntoa(src), src_port,
                inet_ntoa(dst), dst_port,
                data_size);
        fflush(logfile); // Ensure data is written immediately

        // Optional: Print on screen
        printf("?? Packet captured (%s): %s:%d -> %s:%d (%d bytes)\n",
               protocol,
               inet_ntoa(src), src_port,
               inet_ntoa(dst), dst_port,
               data_size);

        // Parse the packet (for terminal display/debugging)
        parse_packet(buffer, data_size);
    }

    close(raw_socket);
    free(buffer);
    fclose(logfile);
}

