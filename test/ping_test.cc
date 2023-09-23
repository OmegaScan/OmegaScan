#include <iostream>

#include "pinger.hh"
using namespace std;

using namespace std;

void print(const pinger &p);

int main(int argc, char const *argv[]) {
  pinger p("39.104.55.143");

  int ret = p.ping_for_success(0.2, 2);

  if (ret == 0) {
    cout << "PING成功" << endl;
    print(p);
  }

  if (ret == -1) {
    cout << "PING失败" << endl;
  }

  return 0;
}

void print(const pinger &p) {
  cout << "ip: " << p.get_addr() << endl;
  cout << "seq: " << p.get_seq() << endl;
  cout << "time: " << (pinger::get_timestamp() - p.get_sending_ts()) * 1000
       << "ms" << endl;
}
