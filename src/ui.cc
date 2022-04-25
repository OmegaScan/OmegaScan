#include "ui.hh"
#include <iostream>
#include <vector>

ui::ui() {
}

ui::~ui() {
}

void ui::showScanning(std::string ip, uint16_t port) {
    std::cout << std::endl;
    std::cout << "scanning: " << "ip: " << ip << "\tport: " << port << std::endl;
}

void ui::showScanning(std::string ip, std::vector<uint16_t> ports) {
    std::cout << std::endl;
    std::cout << "scanning: " << "ip: " << ip << "\tport: ";
    if (ports.size() <= 3) {
        for(auto port : ports) {
            std::cout << port << ",";
        }
    } else { /* 如果端口太多，就只显示几个 */
        for (int i = 0; i < 3; i++) {
            std::cout << ports[i] << ",";
        }
        std::cout << "..." << ports[ports.size() - 1] << std::endl;
    }
}

void ui::showMessage(std::string tip) {
    std::cout << "result: " << tip << std::endl;
}

void ui::showProcess(double process) {
    std::cout << "process: " << process * 100 << "%" << std::endl;
}
