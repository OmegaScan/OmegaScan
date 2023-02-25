#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <vector>

int udp_scanner(std::string host, unsigned short port);