#include "scan.hh"
using namespace std;

int main() {
  vector<string> ips = {"39.104.55.143", "8.8.8.8", "127.0.0.1"};
  vector<unsigned short> ports = {20, 21, 53, 90, 200, 343, 1032, 54544};
  // tcp_syn_scan(ips, ports);
  tcp_ack_scan(ips, ports);
  return 0;
}
