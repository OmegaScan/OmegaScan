#include "ui.hh"
#include <iostream>

ui::ui() {
}

ui::~ui() {
}

void ui::showScanning(std::string ip, uint16_t port) {
    std::cout << std::endl;
    std::cout << "scanning: " << "ip: " << ip << "\tport: " << port << std::endl;
}

void ui::showMessage(std::string tip) {
    std::cout << "result: " << tip << std::endl;
}

void ui::showProcess(double process) {
    std::cout << "process: " << process * 100 << "%" << std::endl;
}
