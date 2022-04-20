#include "omega_main.hh"
#include "tcp_syn.hh"

void omega_main(std::string raw_target_host, std::string raw_port_specified, unsigned int raw_options) {
    std::cout << std::endl;
    std::cout << "omega_main:" << std::endl;
    std::cout << "\ttarget_host: " << raw_target_host << std::endl;
    std::cout << "\tport_specified: " << raw_port_specified << std::endl;
    std::cout << "\toptions: " << raw_options << std::endl;
    std::cout << std::endl;

    prase_host(raw_target_host);

    tcp_syn(raw_target_host, 80);

    return;
}

std::vector<std::string> prase_host(std::string raw_target_host) {
    std::vector<std::string> hosts;
    char start[4] = {};


    std::cout << "This is parse host func" << std::endl;

    return hosts;
}