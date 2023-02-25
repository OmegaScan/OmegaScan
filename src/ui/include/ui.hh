#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class ui {
 private:
 public:
  void update();
  std::string fill(std::string strm, int n);
  void show_scanning(std::string ip, uint16_t port);
  void show_scanning(std::string ip, std::vector<uint16_t> port);
  void show_message(std::string message);
  void show_process(double process);
  void show_successes();

  ui();
  ~ui();
};
