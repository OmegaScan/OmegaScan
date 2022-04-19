#include <string>
#include <iostream>
#include <sys/socket.h>
#include "basic.hh"
#include "omega_main.hh"

std::string raw_target_host;
std::string raw_port_specified;
unsigned int raw_options = 0;

void omega_main() {
    std::cout << std::endl;
    std::cout << "omega_main:" << std::endl;
    std::cout << "\ttarget_host: " << raw_target_host << std::endl;
    std::cout << "\tport_specified: " << raw_port_specified << std::endl;
    std::cout << "\toptions: " << raw_options << std::endl;
    std::cout << std::endl;
    return;
}
