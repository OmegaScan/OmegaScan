#include "omega_main.hh"
#include "parser.hh"
#include "scan.h"
#include "ftxui.hh"
#include "basic.hh"

void omega_main(std::string raw_target_host, std::string raw_port_specified, unsigned int raw_options) {
    // ftxui_init();

    debug(std::cout << std::endl)
    debug(std::cout << "omega_main:" << std::endl)
    debug(std::cout << "\ttarget_host: " << raw_target_host << std::endl)
    debug(std::cout << "\tport_specified: " << raw_port_specified << std::endl)
    debug(std::cout << "\toptions: " << raw_options << std::endl)
    debug(std::cout << std::endl)

    std::vector<std::string> ips = parse_host(raw_target_host);
    std::vector<uint16_t> ports = parse_port(raw_port_specified);

    switch (raw_options) {
    case scan_type::UDP_SCAN:
        udp_scan(ips, ports);
        break;
    case scan_type::TCP_SYN_SCAN:
        tcp_syn_scan(ips, ports);
        break;
    case scan_type::TCP_CNN_SCAN:
        tcp_cnn_scan(ips, ports);
        break;
    case scan_type::TCP_ACK_SCAN:
        tcp_ack_scan(ips, ports);
        break;
    case scan_type::PING_SWEEP:
        ping_sweep(ips);
        break;
    default:
        std::cerr << "scan type error, type --help for usage." << std::endl;
        break;
    }

    return;
}
