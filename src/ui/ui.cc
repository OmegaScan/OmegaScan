#include "ui.hh"

#include <cstdio>
#include <iostream>

using namespace std;

void ui::update() {}
// string ui::fill(string strm, int n);
void ui::show_scanning(string ip, uint16_t port) {}
void ui::show_scanning(string ip, vector<uint16_t> port) {}
void ui::show_message(string message) { printf("%s\n", message.c_str()); }
void ui::show_process(double process) {}
void ui::show_successes() {}

ui::ui() {}
ui::~ui() {}
