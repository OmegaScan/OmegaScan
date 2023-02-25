#include <iostream>

#include "tcp_scanner.hh"

using namespace std;

void handleSuccess(uint32_t ip, uint16_t port, int fd) {
  int p1 = (u_char)((ip >> 24) & 0xFF);
  int p2 = (u_char)((ip >> 16) & 0xFF);
  int p3 = (u_char)((ip >> 8) & 0xFF);
  int p4 = (u_char)(ip & 0xFF);

  printf("%d.%d.%d.%d\t", p1, p2, p3, p4);
  printf("port=%d\tfd=%d\n", port, fd);
}

int main(int argc, char const *argv[]) {
  vector<uint16_t> ports = {80,   81,   88,   443,  8083, 8080,
                            8001, 8888, 3128, 3124, 3000, 1080};

  vector<const char *> ip_addr = {"39.104.55.143", "220.181.38.251",
                                  "109.244.194.121"};

  tcp_scanner s;
  s.set_timeout({0, 500000});
  s.set_success_callback(handleSuccess);

  for (int i = 0; i < ip_addr.size(); i++) {
    uint32_t ip = ntohl(inet_addr(ip_addr[i]));
    s.scan(ip, ports);
  }

  return 0;
}
