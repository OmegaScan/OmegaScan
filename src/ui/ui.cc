#include "ui.hh"

#include <cstdio>
#include <iostream>

void ui::update() {}
// std::string ui::fill(std::string strm, int n);
void ui::showScanning(std::string ip, uint16_t port) {}
void ui::showScanning(std::string ip, std::vector<uint16_t> port) {}
void ui::showMessage(std::string message) { 
    printf("%s\n", message.c_str());
}
void ui::showProcess(double process) {}
void ui::showSuccesses() {}

ui::ui() {}
ui::~ui() {}
