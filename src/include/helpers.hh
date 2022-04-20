#ifndef HELPERS
#define HELPERS

#include <string>
#include <iostream>
#include <strings.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "basic.hh"
int get_sockaddr(std::string host, unsigned short port, struct sockaddr_in* addr_p);

#endif
