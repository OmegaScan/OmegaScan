#include <iostream>
#include <vector>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

int udp_scanner(std::string host, unsigned short port);