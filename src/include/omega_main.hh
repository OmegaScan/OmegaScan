#ifndef OMEGA_MAIN
#define OMEGA_MAIN

#include <string>
#include <iostream>
#include <sys/socket.h>
#include <vector>

#include "basic.hh"
#include "tcp_syn.hh"

void omega_main(std::string raw_target_host, std::string raw_port_specified, int raw_options);
std::vector<std::string> prase_host(std::string raw_target_host);

#endif
