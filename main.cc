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
    std::cout << "-sS/sT/sA/sW/sM: TCP SYN/Connect()/ACK/Window/Maimon scans" << std::endl;
    std::cout << "-sN/sF/sX: TCP Null, FIN, and Xmas scans" << std::endl;
    std::cout << "-sU: UDP Scan" << std::endl;
}

extern std::string raw_target_host;
extern std::string raw_port_specified;
extern unsigned int raw_options;

int main(int argc, char* argv[]) {
    while (true) {
        int optIndex = 0;
        int opt = 0;
        static struct option longOpts[] = {
            { "sS", no_argument, &opt, 2 },
            { "sT", no_argument, &opt, 4 },
            { "sA", no_argument, &opt, 8 },
            { "sU", no_argument, &opt, 1 },
            { "target", required_argument, NULL, 'T' },
            { "version", no_argument, NULL, 'v' },
            { "help", no_argument, NULL, 'h' },
            { 0, 0, 0, 0 }
        };

        int c = getopt_long(argc, argv, "vhp:T:", longOpts, &optIndex);
        // std::cout << "c: " << c << std::endl;
        if (c == -1) {
            break;
        }

        switch (c) {
        case 0: {
            raw_options += opt;
            break;
        }
        case 'p': {
            // std::cout << "Prot Specified: " << optarg << std::endl;
            raw_port_specified = optarg;
            break;
        }
        case 'T': {
            // std::cout << "Target Specified: " << optarg << std::endl;
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

    omega_main();
    return 0;
}
