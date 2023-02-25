#include <iostream>
#include <vector>

#include "ui.hh"

ui::ui() {
  scanning_com = ftxui::hbox({
      text("scanning ip: null \tport: null"),
  });

  process_com = ftxui::hbox({
      gaugeRight(0) | flex,
      text(""),
  });
}

ui::~ui() {}

void ui::update() {
  auto document =
      vbox({scanning_com | border, process_com, message_com.Render()}) |
      borderHeavy;

  Render(screen, document);
  std::cout << reset_position;
  screen.Print();
  reset_position = screen.ResetPosition(); /*重置窗口位置*/
}

void ui::showScanning(std::string ip, uint16_t port) {
  scanning_com = hbox({text(fill("ip:   " + ip, 20)) | bgcolor(0x9400D3_rgb) |
                           borderRounded | flex,
                       text(fill("port:   " + std::to_string(port), 20)) |
                           bgcolor(0x9400D3_rgb) | borderRounded | flex});
  update();
}

void ui::showScanning(std::string ip, std::vector<uint16_t> ports) {
  std::stringstream message;

  if (ports.size() <= 3) {
    for (auto port : ports) {
      message << port << ",";
    }
  } else { /* 如果端口太多，就只显示部分 */
    message << ports[0] << "," << ports[1] << "..." << ports[ports.size() - 1];
  }

  scanning_com = hbox({text(fill("ip:   " + ip, 20)) | bgcolor(0x9400D3_rgb) |
                           borderRounded | flex,
                       text(fill("port:   " + message.str(), 28)) |
                           bgcolor(0x9400D3_rgb) | borderRounded | flex});
  update();
}

void ui::showMessage(std::string message) {
  std::string str = fill(message, 100);
  message_com.addLine(str);
  message_com.setSelectedToLast();

  messages.push_back(text(str));
}

void ui::showProcess(double process) {
  int percentage = (int)(process * 100);
  std::string str = std::to_string(percentage);
  process_com = hbox({
      gaugeRight(process) | flex,
      text(str),
  });
  update();
}

void ui::showSuccesses() {
  auto screen2 = ScreenInteractive::TerminalOutput();
  screen2.ScreenInteractive::Fullscreen();
  screen2.Loop(message_com.getComponent()); /*总结输出结果，向上查找历史记录*/
}

std::string ui::fill(std::string strm, int n) {
  std::string str = strm;
  if (str.length() < n) {
    for (int i = 0; i < n - str.length(); i++) {
      str = str + " ";
    }
  }
  return str;
}
