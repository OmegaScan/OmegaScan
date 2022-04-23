#include <getopt.h>
#include <iostream>
#include <string>
#include "basic.hh"
#include "omega_main.hh"

#define VERSION "0.00"
void print_version() {
    std::cout << "Welcome to OmegaScan! VERSION: " << VERSION << std::endl;
}

void print_usage() {
    std::cout << "Usage: omegascan [Scan Type(s)] [Target]" << std::endl;
    std::cout << "SCAN TECHNIQUES:" << std::endl;
    std::cout << "--sS/sT/sA/sW/sM: TCP SYN/Connect()/ACK/Window/Maimon scans" << std::endl;
    std::cout << "--sN/sF/sX: TCP Null, FIN, and Xmas scans" << std::endl;
    std::cout << "--sU: UDP Scan" << std::endl;
}

int main(int argc, char* argv[]) {

    std::string raw_target_host;
    std::string raw_port_specified;
    unsigned int raw_options = 0;

    while (true) {
        int opt_index = 0;
        int opt = 0;
        static struct option long_opts[] = {
            { "sS", no_argument, &opt, scan_type::TCP_SYN_SCAN },
            { "sT", no_argument, &opt, scan_type::TCP_CNN_SCAN },
            { "sA", no_argument, &opt, scan_type::TCP_ACK_SCAN },
            { "sU", no_argument, &opt, scan_type::UDP_SCAN },
            { "target", required_argument, NULL, 'T' },
            { "version", no_argument, NULL, 'v' },
            { "help", no_argument, NULL, 'h' },
            { 0, 0, 0, 0 }
        };

        int c = getopt_long(argc, argv, "vhp:T:", long_opts, &opt_index);
        if (c == -1) {
            break;
        }

        switch (c) {
        case 0: {
            raw_options += opt;
            break;
        }
        case 'p': {
            raw_port_specified = optarg;
            break;
        }
        case 'T': {
            raw_target_host = optarg;
            break;
        }
        case 'v': {
            print_version();
            exit(0);
        }
        case 'h': {
            print_version();
            print_usage();
            exit(0);
        }
        default: {
            print_usage();
            exit(0);
        }
        }
    }

    omega_main(raw_target_host, raw_port_specified, raw_options);
    return 0;
}
