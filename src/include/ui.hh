#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <sstream>

#include <chrono>                  // for operator""s, chrono_literals
#include <iostream>                 // for cout, endl, ostream
#include <string>  // for allocator, operator+, char_traits, operator<<, string, to_string, basic_string
#include <thread>  // for sleep_for

#include "ftxui/dom/node.hpp"      // for Render
#include "ftxui/screen/color.hpp"  // for ftxui
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/screen/string.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive

using namespace ftxui;
using namespace std::chrono_literals;

class MyMenu {
private:
    std::vector<std::string> lines;
    int selected_line = 0;
    Component base_menu = Menu(&lines, &selected_line);
    Component my_menu;

public:
    void addLine(std::string l) {
        lines.push_back(l);
    }
    void setSelected(int index) {
        selected_line = index >= 0 ? index : 0;
    }
    void setSelectedToLast() {
        selected_line = lines.size() > 0 ? lines.size() - 1 : 0;
    }
    Component getComponent() {
        return my_menu;
    }
    Element Render() {
        return my_menu->Render();
    }
    MyMenu();
    ~MyMenu() {}
};

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
