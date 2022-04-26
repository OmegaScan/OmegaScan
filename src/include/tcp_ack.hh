#ifndef TCP_ACK
#define TCP_ACK

#include <iostream>
#include <strings.h>
#include <sys/socket.h>
#include <string>
#include <arpa/inet.h>
#include "basic.hh"

int tcp_ack(std::string host, unsigned short port, unsigned short local_port = LOCAL_PORT);

#endif
