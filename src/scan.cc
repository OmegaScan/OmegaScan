#include "scan.h"
#include "tcp_ack.hh"
#include "tcp_scanner.h"
#include "tcp_syn.hh"
#include "udp_scanner.hh"
#include "ui.hh"
#include "basic.hh"
#include "helpers.hh"
#include <sstream>

ui my_ui; // 本来不想用全局变量的，但是由于C的回调函数...

void tcp_syn_scan(std::vector<std::string> ips, std::vector<uint16_t> ports) {

    int scanned_count = 0;

    for (int i = 0; i < ips.size(); i++) {
        for (int j = 0; j < ports.size(); j++) {
            my_ui.showScanning(ips[i], ports[j]);
            int ret = tcp_syn(ips[i], ports[j]);

            // 生成扫描结果字符串
            std::stringstream message;
            switch (ret) {
            case syn_res::RST:
                message << "s! "
                        << "ip: " << ips[i] << "\t"
                        << "port: " << ports[j];
                break;
            case syn_res::ACK:
                message << "s! "
                        << "ip: " << ips[i] << "\t"
                        << "port: " << ports[j];
                break;
            default:
                message << "f! "
                        << "ip: " << ips[i] << "\t"
                        << "port: " << ports[j];
                break;
            }
            my_ui.showMessage(message.str());

            my_ui.showProcess((double)++scanned_count / (double)(ips.size() * ports.size()));
        }
    }
    my_ui.showSuccesses();
}

void tcp_cnn_scan_success_handler(u_long ip, int port, int fd) {

    int p1 = (u_char)((ip >> 24) & 0xFF);
    int p2 = (u_char)((ip >> 16) & 0xFF);
    int p3 = (u_char)((ip >> 8) & 0xFF);
    int p4 = (u_char)(ip & 0xFF);

    std::stringstream message;
    message << "success! "
            << "ip=" << p1 << "." << p2 << "." << p3 << "." << p4 << "\t"
            << "port=" << port;
    my_ui.showMessage(message.str());
}

void tcp_cnn_scan(std::vector<std::string> ips, std::vector<uint16_t> ports) {

    // tcpScanner::scan的参数与当前函数的入参不兼容，做一层转换
    std::vector<int> ports_int(ports.size());
    for (int i = 0; i < ports.size(); i++) {
        ports_int[i] = ports[i];
    }
    std::vector<u_long> ips_ulong(ips.size());
    for (int i = 0; i < ips.size(); i++) {
        ips_ulong[i] = ntohl(inet_addr(ips[i].c_str()));
    }

    tcpScanner scanner;
    scanner.setTimeout({1, 0});
    scanner.setSuccessCallback(tcp_cnn_scan_success_handler);

    for (int i = 0; i < ips.size(); i++) {
        if (ports_int.size() <= 300) {
            my_ui.showScanning(ips[i], ports[0]); // TODO 由于同时操作多个socket，这里应该显示多个端口，而不是一个，待会改一下ui类
            scanner.scan(ips_ulong[i], ports_int);
            my_ui.showProcess((double)(i + 1) / (double)ips.size());
        } else {
            // TODO 大于300个端口应该分割再扫描，但由于port端口一个用的u_int32一个用的long int，这里不好写，等待会统一数据类型后再写
        }
    }
}

void tcp_ack_scan(std::vector<std::string> ips, std::vector<uint16_t> ports) {
    // TODO 等ack_test写好后再写
}

void udp_scan(std::vector<std::string> ips, std::vector<uint16_t> ports) {
    int scanned_count = 0;

    for (int i = 0; i < ips.size(); i++) {
        for (int j = 0; j < ports.size(); j++) {

            my_ui.showScanning(ips[i], ports[j]);
            int ret = udp_scanner(ips[i], ports[j]);

            std::stringstream message;
            switch (ret) {
            case -1:
                message << "ip: " << ips[i] << "\t"
                        << "port: " << ports[j] << "\t"
                        << "is open/filtered";
                break;
            case 1:
                message << "ip: " << ips[i] << "\t"
                        << "port: " << ports[j] << "\t"
                        << "is closed";
                break;
            default:
                message << "unknow error! "
                        << "ip: " << ips[i] << "\t"
                        << "port: " << ports[j];
                break;
            }
            my_ui.showMessage(message.str());
            my_ui.showProcess((double)++scanned_count / (double)(ips.size() * ports.size()));
        }
    }
}

void ping_sweep(std::vector<std::string> ips) {
    // TODO 待会写
}