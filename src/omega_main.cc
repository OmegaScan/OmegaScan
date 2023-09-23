#include "omega_main.hh"

#include "basic.hh"
#include "ftxui.hh"
#include "parser.hh"
#include "scan.hh"

using namespace std;

void omega_main(string raw_target, string raw_port, unsigned int raw_options) {
  debug(cout << endl);
  debug(cout << "omega_main:" << endl);
  debug(cout << "\ttarget_host: " << raw_host << endl);
  debug(cout << "\tport_specified: " << raw_port << endl);
  debug(cout << "\toptions: " << raw_options << endl);
  debug(cout << endl);

  vector<string> ips = parse_host(raw_target);
  vector<uint16_t> ports = parse_port(raw_port);

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
      cerr << "scan type error, type --help for usage." << endl;
      break;
  }

  return;
}
