#ifndef HELPERS
#define HELPERS

#include <arpa/inet.h>
#include <strings.h>
#include <sys/socket.h>

#include <iostream>
#include <string>
#include <vector>

struct psdhdr {
  uint32_t saddr;  // 源地址
  uint32_t daddr;  // 目的地址
  uint8_t mbz;     // 强制置空
  uint8_t ptcl;    // 协议类型
  uint16_t tcpl;   // TCP长度
};

void perror_exit(const char *s);

int get_sockaddr(std::string host, unsigned short port,
                 struct sockaddr_in *addr_p);

std::string get_local_ip();

void set_ip_hdr(struct iphdr *ip_header, std::string target_addr);

void set_tcp_hdr(struct tcphdr *tcp_header, unsigned short dst_port,
                 unsigned short src_port, int flags);

uint16_t checksum(const void *data, const int length);

uint16_t tcp_checksum(struct iphdr *ip_header, struct tcphdr *tcp_header);

void print_hdr_msg(char *buffer);

unsigned int get_flag_of(char *buf, size_t size);

template <typename T>
std::vector<std::vector<T>> &vector_slice(std::vector<T> &vec,
                                          size_t fineness) {
  size_t pieces = vec.size() / fineness + (vec.size() % fineness != 0);
  auto result = new std::vector<std::vector<T>>;
  auto vec_iter = vec.begin();
  int offset_begin;
  int offset_end;

  for (int i = 0; i < pieces; i++) {
    offset_begin = i * fineness;
    offset_end = offset_begin + fineness;
    offset_end = offset_end < vec.size() ? offset_end : vec.size();
    std::vector<T> temp;
    temp.insert(temp.begin(), vec_iter + offset_begin, vec_iter + offset_end);
    result->push_back(temp);
  }

  return *result;
}

// std::vector<std::vector<uint16_t>>& vector_slice(std::vector<uint16_t>& vec,
// size_t fineness);
#endif
