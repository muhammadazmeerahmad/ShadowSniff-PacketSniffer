#include <stdio.h>
#include <string.h>
#include <netinet/ip.h>        // struct iphdr
#include <netinet/tcp.h>       // struct tcphdr
#include <netinet/udp.h>       // struct udphdr
#include <netinet/ip_icmp.h>   // struct icmphdr
#include <net/ethernet.h>      // struct ethhdr
#include <arpa/inet.h>         // inet_ntoa()
#include "protocol_parser.h"   // Custom header

// Main parser that handles Ethernet and IP header, and dispatches to the correct protocol
void parse_packet(const unsigned char *buffer, int size) {
    struct ethhdr *eth = (struct ethhdr *)buffer;

    // Check if the packet is an IP packet (EtherType = 0x0800)
    if (ntohs(eth->h_proto) == 0x0800) {
        struct iphdr *iph = (struct iphdr *)(buffer + sizeof(struct ethhdr));

        struct sockaddr_in source, dest;
        memset(&source, 0, sizeof(source));
        source.sin_addr.s_addr = iph->saddr;

        memset(&dest, 0, sizeof(dest));
        dest.sin_addr.s_addr = iph->daddr;

        printf("\n?? IP Packet: %s ? %s | Protocol: %d\n",
               inet_ntoa(source.sin_addr),
               inet_ntoa(dest.sin_addr),
               iph->protocol);

        // Route to protocol-specific parser
        switch (iph->protocol) {
            case IPPROTO_TCP:
                parse_tcp(buffer + sizeof(struct ethhdr), size - sizeof(struct ethhdr));
                break;
            case IPPROTO_UDP:
                parse_udp(buffer + sizeof(struct ethhdr), size - sizeof(struct ethhdr));
                break;
            case IPPROTO_ICMP:
                parse_icmp(buffer + sizeof(struct ethhdr), size - sizeof(struct ethhdr));
                break;
            default:
                printf("?? Unknown or unsupported protocol\n");
                break;
        }
    } else {
        printf("?? Non-IP packet (EtherType: 0x%04x), skipping.\n", ntohs(eth->h_proto));
    }
}

// Parses TCP header fields
void parse_tcp(const unsigned char *buffer, int size) {
    struct iphdr *iph = (struct iphdr *)buffer;
    unsigned short iphdr_len = iph->ihl * 4;

    struct tcphdr *tcph = (struct tcphdr *)(buffer + iphdr_len);

    printf("?? TCP Segment:\n");
    printf("    +- Source Port      : %u\n", ntohs(tcph->source));
    printf("    +- Destination Port : %u\n", ntohs(tcph->dest));
    printf("    +- Sequence Number  : %u\n", ntohl(tcph->seq));
    printf("    +- Ack Number       : %u\n", ntohl(tcph->ack_seq));
}

// Parses UDP header fields
void parse_udp(const unsigned char *buffer, int size) {
    struct iphdr *iph = (struct iphdr *)buffer;
    unsigned short iphdr_len = iph->ihl * 4;

    struct udphdr *udph = (struct udphdr *)(buffer + iphdr_len);

    printf("?? UDP Datagram:\n");
    printf("    +- Source Port      : %u\n", ntohs(udph->source));
    printf("    +- Destination Port : %u\n", ntohs(udph->dest));
    printf("    +- Length           : %u\n", ntohs(udph->len));
}

// Parses ICMP header fields
void parse_icmp(const unsigned char *buffer, int size) {
    struct iphdr *iph = (struct iphdr *)buffer;
    unsigned short iphdr_len = iph->ihl * 4;

    struct icmphdr *icmph = (struct icmphdr *)(buffer + iphdr_len);

    printf("?? ICMP Packet:\n");
    printf("    +- Type     : %d\n", (unsigned int)(icmph->type));
    printf("    +- Code     : %d\n", (unsigned int)(icmph->code));
    printf("    +- Checksum : %d\n", ntohs(icmph->checksum));
}

