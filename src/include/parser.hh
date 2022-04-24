#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <sys/socket.h>
#include <arpa/inet.h>

#define LAST_HOST "255.255.255.255"
#define LAST_PORT "65535"
#define delimiter '-'

std::vector<std::string> parse_host(const std::string &raw_target_host);

std::vector<uint16_t> parse_port(const std::string &raw_port_specified);