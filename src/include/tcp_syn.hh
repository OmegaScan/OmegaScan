#ifndef TCP_SYN
#define TCP_SYN

#include <iostream>
#include <strings.h>
#include <sys/socket.h>
#include <string>
#include <arpa/inet.h>

int tcp_syn(std::string host, unsigned short port);

#endif
