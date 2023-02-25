#ifndef OMEGA_MAIN
#define OMEGA_MAIN

#include <iostream>
#include <string>

#include "basic.hh"
#include "tcp_syn.hh"

void omega_main(std::string raw_target_host, std::string raw_port_specified,
                unsigned int raw_options);

#endif