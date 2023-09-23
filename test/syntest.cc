#include <unistd.h>

#include <iostream>

#include "tcp_syn.hh"
using namespace std;

int main(int argc, char *argv[]) {
  cout << "21 port: " << tcp_syn("39.104.55.143", 21, LOCAL_PORT) << endl;
  sleep(1);
  cout << "22 port: " << tcp_syn("39.104.55.143", 22, LOCAL_PORT) << endl;
  sleep(1);
  cout << "80 port: " << tcp_syn("39.104.55.143", 80, LOCAL_PORT) << endl;
  sleep(1);
  cout << "443 port: " << tcp_syn("39.104.55.143", 443, LOCAL_PORT) << endl;
  sleep(1);
  cout << "1234 port: " << tcp_syn("39.104.55.143", 1234, LOCAL_PORT) << endl;
  return 0;
}
