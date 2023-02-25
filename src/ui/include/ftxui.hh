#include <unistd.h>

#include <chrono>  // for operator""s, chrono_literals
#include <iostream>
#include <iostream>  // for cout, endl, ostream
#include <sstream>
#include <string>
#include <string>  // for allocator, operator+, char_traits, operator<<, string, to_string, basic_string
#include <thread>  // for sleep_for
#include <vector>

#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"      // for Render
#include "ftxui/screen/color.hpp"  // for ftxui
#include "ftxui/screen/screen.hpp"
#include "ftxui/screen/string.hpp"

using namespace ftxui;
using namespace std::chrono_literals;

class MyMenu {
 private:
  std::vector<std::string> lines;
  int selected_line = 0;
  Component base_menu = Menu(&lines, &selected_line);
  Component my_menu;

 public:
  void addLine(std::string l) { lines.push_back(l); }
  void setSelected(int index) { selected_line = index >= 0 ? index : 0; }
  void setSelectedToLast() {
    selected_line = lines.size() > 0 ? lines.size() - 1 : 0;
  }
  Component getComponent() { return my_menu; }
  Element Render() { return my_menu->Render(); }
  MyMenu() {
    my_menu = Renderer(base_menu, [&] {
      int begin = 0;
      int end = 0;
      if (selected_line < 4) {
        begin = 0;
        end = std::min(8, (int)(lines.size() - 1));
      } else if (selected_line + 4 >= lines.size()) {
        begin = std::max((int)(lines.size() - 9), 0);
        end = lines.size() - 1;
      } else {
        begin = selected_line - 4;
        end = selected_line + 4;
      }

      Elements elements;
      for (int i = begin; i <= end; ++i) {
        Element element = text(lines[i]);
        if (i == selected_line) element = element | inverted;
        elements.push_back(element);
      }

      if (end - begin < 8) {
        int blank_count = 9 - (end - begin + 1);
        for (int i = 0; i < blank_count; i++) {
          elements.push_back(text(""));
        }
      }
      return vbox(std::move(elements)) | vscroll_indicator | frame | border;
    });
  }
  ~MyMenu() {}
};

class ui {
 private:
  ftxui::Screen screen =
      Screen::Create(Dimension::Full(), Dimension::Fixed(20));
  std::string reset_position;
  ftxui::Element scanning_com;
  ftxui::Element process_com;
  ftxui::Elements messages;
  MyMenu message_com;

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
