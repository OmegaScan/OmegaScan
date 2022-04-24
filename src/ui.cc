#include "ui.hh"
#include <iostream>

ui::ui() {
}

ui::~ui() {
}

void ui::showScanning(std::string ip, uint16_t port) {
    std::cout << std::endl;
    std::cout << "正在扫描：" << "ip: " << ip << "\tport: " << port << std::endl;
}

void ui::showMessage(std::string tip) {
    std::cout << "扫描成功：" << tip << std::endl;
}

void ui::showProcess(double process) {
    std::cout << "进度：" << process * 100 << "%" << std::endl;
}
