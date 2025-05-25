#ifndef PROTOCOL_PARSER_H
#define PROTOCOL_PARSER_H

// Function declarations for protocol parsing

// Main function to parse the packet
void parse_packet(const unsigned char *buffer, int size);

// Functions to parse each protocol
void parse_tcp(const unsigned char *buffer, int size);
void parse_udp(const unsigned char *buffer, int size);
void parse_icmp(const unsigned char *buffer, int size);

#endif

