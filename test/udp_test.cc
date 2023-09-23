#include "udp_scanner.hh"
using namespace std;

int main(int argc, char const *argv[]) {
  string host = "39.104.55.143";
  vector<unsigned short> ports = {8890, 8894, 8899, 8900, 9000};
  for (unsigned short port : ports) {
    int ret = udp_scanner(host, port);
    if (-1 == ret) {
      cout << "port " << port << " is open/filtered" << endl;
    } else if (1 == ret) {
      cout << "port " << port << " is closed" << endl;
    }
  }
  return 0;
}
