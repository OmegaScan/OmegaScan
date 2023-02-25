#include "ui.hh"

#include <cstdio>
#include <iostream>

void ui::update() {}
// std::string ui::fill(std::string strm, int n);
void ui::show_scanning(std::string ip, uint16_t port) {}
void ui::show_scanning(std::string ip, std::vector<uint16_t> port) {}
void ui::show_message(std::string message) { printf("%s\n", message.c_str()); }
void ui::show_process(double process) {}
void ui::show_successes() {}

ui::ui() {}
ui::~ui() {}
