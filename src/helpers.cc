#include <arpa/inet.h>
#include <cstring>
#include <ifaddrs.h>
#include <iostream>
#include <net/if.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <string>
#include <sys/ioctl.h>

#include "helpers.hh"
#include "basic.hh"

void perror_exit(const char* s) {
    fprintf(stderr, "%s: %s\n", s, strerror(errno));
    exit(EXIT_FAILURE);
}

int get_sockaddr(std::string host, unsigned short port, struct sockaddr_in* addr_p) {
    if (port < 1 || port > 65535)
        return error_type::PORT_OUT_OF_RANGE;
    struct sockaddr_in addr = *addr_p;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_aton(host.c_str(), &(addr.sin_addr)) == 0) {
        return error_type::TARGET_RRASE_FAILED;
    };
    return 0;
}

std::string get_local_ip() {
    struct ifaddrs* ifap = NULL;
    void* tmp_addr_ptrt = NULL;
    char addressBuffer[INET_ADDRSTRLEN];

    getifaddrs(&ifap);

    while (ifap != NULL) {
        if (ifap->ifa_addr->sa_family == AF_INET) { // check it is IP4
            /* Is a valid IP4 Address */
            tmp_addr_ptrt = &((struct sockaddr_in*)ifap->ifa_addr)->sin_addr;
            char address_buffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmp_addr_ptrt, address_buffer, INET_ADDRSTRLEN);
            if (strcmp(address_buffer, "127.0.0.1") != 0) {
                return std::string(address_buffer);
            }
        }
        ifap = ifap->ifa_next;
    }
    return "";
}

void set_ip_hdr(struct iphdr* ip_header, std::string target_addr) {
    ip_header->version = 4;       /* IPv4 */
    ip_header->ihl = 5;           /* (5 * 4) = 20 bytes; no options */

    ip_header->tos = 0x0;         /* Type of service */
    ip_header->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr);    /* No payload */
    ip_header->id= htons(CUSTOM_IP_ID);

    ip_header->frag_off = 0;
    ip_header->ttl = CUSTOM_IP_TTL;      /* Time to live */
    ip_header->protocol = IPPROTO_TCP;
    ip_header->check = 0;   /* Will be cacl later */

    inet_pton(AF_INET, get_local_ip().c_str(), &(ip_header->saddr));
    inet_pton(AF_INET, target_addr.c_str(), &(ip_header->daddr));
}

void set_tcp_hdr(struct tcphdr* tcp_header, unsigned short dst_port, unsigned short src_port, int flags) {
    tcp_header->th_dport = htons(dst_port);
    tcp_header->th_sport = htons(src_port);
    tcp_header->th_seq = htonl(TCP_SEQ_NUM);
    tcp_header->th_ack = htonl(0);
    tcp_header->th_x2 = 0;
    tcp_header->th_off = 5;     /* Assuming the minimal header length */
    tcp_header->th_flags = (uint8_t)flags;
    tcp_header->th_win = htonl(TCP_WIN_SIZE);
    tcp_header->th_sum = 0;     /* Will compute after header is completly set */
    tcp_header->th_urp = 0;
}

uint16_t checksum(const void* data, const int length) {
    /*  Checksum Algorithm
    (http://www.microhowto.info/howto/calculate_an_internet_protocol_checksum_in_c.html)
    1. Set the sum to 0,
    2. Pad the data to an even number of bytes,
    3. Reinterpret the data as a sequence of 16-bit unsigned integers that are
        in network byte order,
    4. Calculate the sum of the integers, subtracting 0xffff whenever
        the sum => 0x10000, and
    5. Calculate the bitwise complement of the sum and set it as the checksum.
    */
    uint16_t* accumalator = (uint16_t*)data;
    uint64_t sum = 0;

    /* Take care of the first 16-bit even blocks */
    for (int i = 0; i < length / 2; ++i) {
        sum += *(accumalator + i);
        if (sum >= 0x10000) {
            sum -= 0xffff;
        }
    }

    /* Handle the ending partial block */
    if (length % 2 != 0) {
        accumalator = accumalator + length / 2; /* Point accumalator to the end block */
        uint16_t end_block = 0;
        memcpy(&end_block, accumalator, sizeof(uint16_t));
        sum += ntohs(end_block);
        if (sum >= 0x10000) {
            sum -= 0xffff;
        }
    }

    /* Return the one's complement of the checksum in network byte order */
    return htons(~sum);
}

uint16_t tcp_checksum(struct iphdr *ip_header, struct tcphdr *tcp_header) {
    struct psdhdr psh;

    psh.saddr = ip_header->saddr;
    psh.daddr = ip_header->daddr;
    psh.mbz = 0;
    psh.ptcl = IPPROTO_TCP;
    psh.tcpl = htons(sizeof(struct tcphdr)); /* No options, and no data */

    int pseudogram_size = sizeof(struct tcphdr) + sizeof(struct psdhdr);
    char* pseudogram = (char*)malloc(pseudogram_size);

    memcpy(pseudogram, (char*)&psh, sizeof(struct psdhdr));
    memcpy(pseudogram + sizeof(struct psdhdr), tcp_header, sizeof(struct tcphdr));

    return htons(checksum(pseudogram, pseudogram_size));
}
