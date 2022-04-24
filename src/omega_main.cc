#include "omega_main.hh"
#include "tcp_syn.hh"
#include "parser.hh"

void omega_main(std::string raw_target_host, std::string raw_port_specified, unsigned int raw_options) {
    std::cout << std::endl;
    std::cout << "omega_main:" << std::endl;
    std::cout << "\ttarget_host: " << raw_target_host << std::endl;
    std::cout << "\tport_specified: " << raw_port_specified << std::endl;
    std::cout << "\toptions: " << raw_options << std::endl;
    std::cout << std::endl;

    // tcp_syn(raw_target_host, 80);

    std::vector<std::string> vs = parse_host("127.0.0.1");
    for(std::string s : vs) {
        std::cout << s << std::endl;
    }

    return;
}
