#include <getopt.h>

#include <iostream>
#include <string>

#include "basic.hh"
#include "omega_main.hh"

#define VERSION "0.00"

using namespace std;

void print_version() {
  cout << "Welcome to OmegaScan! VERSION: " << VERSION << endl;
}

void print_usage() {
  cout << "Usage: omegascan [Scan Type(s)] [Target]" << endl;
  cout << "SCAN TECHNIQUES:" << endl;
  cout << "--sS/sT/sA: TCP SYN/Connect()/ACK" << endl;
  cout << "--sU: UDP Scan" << endl;
  cout << "--sP: Ping sweep" << endl;
}

int main(int argc, char *argv[]) {
  string raw_target_host;
  string raw_port_specified;
  unsigned int raw_options = 0;

  if (argc == 1) {
    print_usage();
    return 0;
  }

  while (true) {
    int opt_index = 0;
    int opt = 0;
    static struct option long_opts[] = {
        {"sS", no_argument, &opt, scan_type::TCP_SYN_SCAN},
        {"sT", no_argument, &opt, scan_type::TCP_CNN_SCAN},
        {"sA", no_argument, &opt, scan_type::TCP_ACK_SCAN},
        {"sU", no_argument, &opt, scan_type::UDP_SCAN},
        {"sP", no_argument, &opt, scan_type::PING_SWEEP},
        {"target", required_argument, NULL, 'T'},
        {"version", no_argument, NULL, 'v'},
        {"help", no_argument, NULL, 'h'},
        {0, 0, 0, 0}};

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
