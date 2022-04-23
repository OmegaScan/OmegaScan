#ifndef HELPERS
#define HELPERS

#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <strings.h>
#include <sys/socket.h>

struct psdhdr {
    unsigned long saddr; // 源地址
    unsigned long daddr; // 目的地址
    char mbz;            // 强制置空
    char ptcl;           // 协议类型
    unsigned short tcpl; // TCP长度
};

void perror_exit(const char* s);

int get_sockaddr(std::string host, unsigned short port, struct sockaddr_in* addr_p);

std::string get_local_ip();

void set_ip_hdr(struct iphdr* ip_header, std::string target_addr);

void set_tcp_hdr(struct tcphdr* tcp_header, unsigned short dst_port, unsigned short src_port, int flags);

uint16_t checksum(const void* data, const int length);

uint16_t tcp_checksum(struct iphdr* ip_header, struct tcphdr* tcp_header);

#endif
