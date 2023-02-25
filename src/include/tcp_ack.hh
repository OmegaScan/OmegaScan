#ifndef TCP_ACK
#define TCP_ACK

#include <arpa/inet.h>
#include <strings.h>
#include <sys/socket.h>

#include <iostream>
#include <string>

#include "basic.hh"

int tcp_ack(std::string host, unsigned short port,
            unsigned short local_port = LOCAL_PORT);

#endif
