#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <sstream>

class ui {
private:

public:
    void update();
    std::string fill(std::string strm, int n);
    void showScanning(std::string ip, uint16_t port);
    void showScanning(std::string ip, std::vector<uint16_t> port);
    void showMessage(std::string message);
    void showProcess(double process);
    void showSuccesses();

    ui();
    ~ui();
};
