#include <iostream>
#include <vector>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

int udp_scanner(std::string raw_target_host, std::vector<unsigned short> port_list);