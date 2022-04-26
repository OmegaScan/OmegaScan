#include "ui.hh"
#include <iostream>
#include <vector>

auto screen = Screen::Create(
                  Dimension::Full(),
                  Dimension::Fixed(20)
              );

std::string reset_position;

ftxui::Element scanning_com = hbox({
    text("scanning ip: null \tport: null"),
});
ftxui::Element process_com = hbox({
    gaugeRight(0) | flex,
    text(""),
});
ftxui::Elements messages;
MyMenu message_com;

MyMenu::MyMenu() {
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
        for(int i = begin; i <= end; ++i) {
            Element element = text(lines[i]);
            if (i == selected_line)
                element = element | inverted;
            elements.push_back(element);
        }

        if(end - begin < 8) {
            int blank_count = 9 - (end - begin + 1);
            for (int i = 0; i < blank_count; i++) {
                elements.push_back(text(""));
            }
        }
        return vbox(std::move(elements)) | vscroll_indicator | frame | border;
    });
}

ui::ui() {
}

ui::~ui() {
}

void ui::update() {
    auto document = vbox({
        scanning_com | border,
        process_com,
        message_com.Render()
    }) | borderHeavy;

    Render(screen, document);
    std::cout << reset_position;
    screen.Print();
    reset_position = screen.ResetPosition();/*重置窗口位置*/
}

void ui::showScanning(std::string ip, uint16_t port) {
    scanning_com = hbox({
        text(fill("ip:   " + ip, 20)) | bgcolor(0x9400D3_rgb) | borderRounded | flex,
        text(fill("port:   " + std::to_string(port), 20)) | bgcolor(0x9400D3_rgb) |  borderRounded | flex
    });
    update();
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

void ui::showMessage(std::string message) {
    std::string str = fill(message, 100);
    message_com.addLine(str);
    message_com.setSelectedToLast();

    messages.push_back(text(str));
}


void ui::showProcess(double process) {
    int percentage = (int)(process * 100);
    std::string str = std::to_string(percentage);
    process_com =
    hbox({
        gaugeRight(process) | flex,
        text(str),
    });
    update();
}

void ui::showSuccesses() {
    auto screen2 = ScreenInteractive::TerminalOutput();
    screen2.ScreenInteractive::Fullscreen();
    screen2.Loop(message_com.getComponent());/*总结输出结果，向上查找历史记录*/
}

std::string ui::fill(std::string strm, int n) {
    std::string str = strm;
    if(str.length() < n) {
        for(int i = 0; i < n - str.length(); i++) {
            str = str + " ";
        }
    }
    return str;
}
