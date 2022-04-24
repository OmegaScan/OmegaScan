#ifndef HELPERS
#define HELPERS

#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <strings.h>
#include <sys/socket.h>

struct psdhdr {
    uint32_t saddr; // 源地址
    uint32_t daddr; // 目的地址
    uint8_t mbz;    // 强制置空
    uint8_t ptcl;   // 协议类型
    uint16_t tcpl;  // TCP长度
};

void perror_exit(const char* s);

int get_sockaddr(std::string host, unsigned short port, struct sockaddr_in* addr_p);

std::string get_local_ip();

void set_ip_hdr(struct iphdr* ip_header, std::string target_addr);

void set_tcp_hdr(struct tcphdr* tcp_header, unsigned short dst_port, unsigned short src_port, int flags);

uint16_t checksum(const void* data, const int length);

uint16_t tcp_checksum(struct iphdr* ip_header, struct tcphdr* tcp_header);

void print_hdr_msg(char* buffer);

unsigned int get_flag_of(char* buf, size_t size);
#endif
