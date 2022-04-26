#include "scan.h"

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_int_distribution<int> dist(-1000, 1000);
auto rnd = std::bind(dist, mt);
ui my_ui; // 本来不想用全局变量的，但是由于C的回调函数...
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
long countP = 0;

void print_tcp_syn(std::string host, unsigned short port, int total) {

    int ret = tcp_syn(host, port, LOCAL_PORT);

    pthread_mutex_lock(&mutex);
    my_ui.showScanning(host, port);

    // 生成扫描结果字符串
    std::stringstream message;
    switch (ret) {
    case syn_res::RST:
        message << "ip: " << host << ", "
                << "port: " << port << " -> close";
        break;
    case syn_res::ACK:
        message << "ip: " << host << ", "
                << "port: " << port << " -> open";
        break;
    default:
        message << "ip: " << host << ", "
                << "port: " << port << " -> filtered";
        break;
    }
    my_ui.showMessage(message.str());

    countP++;

    my_ui.showProcess((double)countP / (double)total);
    pthread_mutex_unlock(&mutex);
}


void tcp_syn_scan(std::vector<std::string> ips, std::vector<uint16_t> ports) {

    ThreadPool pool(THREAD_NUMBER);
    pool.init();

    int scanned_count = 0;

    for (int i = 0; i < ips.size(); i++) {
        for (int j = 0; j < ports.size(); j++) {
            double process_value = (double)++scanned_count / (double)(ips.size() * ports.size());
            pool.submit(print_tcp_syn, ips[i], ports[j], ips.size() * ports.size());
        }
    }
    auto finish_status = pool.submit(finish_scan);
    finish_status.get();
    pool.shutdown();

    my_ui.showSuccesses();
}


void tcp_cnn_scan_success_handler(uint32_t ip, uint16_t port, int fd) {

    int p1 = (u_char)((ip >> 24) & 0xFF);
    int p2 = (u_char)((ip >> 16) & 0xFF);
    int p3 = (u_char)((ip >> 8) & 0xFF);
    int p4 = (u_char)(ip & 0xFF);

    std::stringstream message;
    message << "ip: " << p1 << "." << p2 << "." << p3 << "." << p4 << ", "
            << "port: " << port << " -> open";
    my_ui.showMessage(message.str());
}

void tcp_cnn_scan(std::vector<std::string> ips, std::vector<uint16_t> ports) {

    tcpScanner scanner;
    scanner.setTimeout({1, 0});
    scanner.setSuccessCallback(tcp_cnn_scan_success_handler);

    int scanned_count = 0;

    for (int i = 0; i < ips.size(); i++) {
        uint32_t ip = ntohl(inet_addr(ips[i].c_str()));
        if (ports.size() <= 300) {
            my_ui.showScanning(ips[i], ports);
            scanner.scan(ip, ports);
            my_ui.showProcess((double)++scanned_count / (double)ips.size());
        } else {
            auto ports_fragments = vector_slice(ports, 300);
            for (auto ports_fragment : ports_fragments) {
                my_ui.showScanning(ips[i], ports_fragment);
                scanner.scan(ip, ports_fragment);
                scanned_count += ports_fragment.size();
                my_ui.showProcess((double)scanned_count / (double)(ips.size() * ports.size()));
            }
        }
    }
    my_ui.showSuccesses();
}

void print_tcp_ack(std::string host, unsigned short port, int total) {

    int ret = tcp_syn(host, port, LOCAL_PORT);

    pthread_mutex_lock(&mutex);
    my_ui.showScanning(host, port);
    std::stringstream message;
    if (ret > 0) {
        message << "ip: " << host << ", "
                << "port: " << port << " -> unfiltered";
    } else {
        message << "ip: " << host << ", "
                << "port: " << port << " -> filtered";
    }
    my_ui.showMessage(message.str());
    countP++;

    my_ui.showProcess((double)countP / (double)total);
    pthread_mutex_unlock(&mutex);
}


void tcp_ack_scan(std::vector<std::string> ips, std::vector<uint16_t> ports) {

    ThreadPool pool(THREAD_NUMBER);

    pool.init();

    int scanned_count = 0;

    for (int i = 0; i < ips.size(); i++) {
        for (int j = 0; j < ports.size(); j++) {
            // double process_value = (double)++scanned_count / (double)(ips.size() * ports.size());
            pool.submit(print_tcp_ack, ips[i], ports[j],  ips.size() * ports.size());
        }
    }
    auto finish_status = pool.submit(finish_scan);
    finish_status.get();
    pool.shutdown();
    my_ui.showSuccesses();
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
                message << "ip: " << ips[i] << ", "
                        << "port: " << ports[j] << " -> open | filtered";
                break;
            case 1:
                message << "ip: " << ips[i] << ", "
                        << "port: " << ports[j] << " -> closed";
                break;
            default:
                message << "ip: " << ips[i] << ", "
                        << "port: " << ports[j] << " -> unkown error";
                break;
            }
            my_ui.showMessage(message.str());
            my_ui.showProcess((double)++scanned_count / (double)(ips.size() * ports.size()));
        }
    }
    my_ui.showSuccesses();
}

void ping_sweep(std::vector<std::string> ips) {
    for (int i = 0; i < ips.size(); i++) {
        my_ui.showScanning(ips[i], 0);
        pinger p(ips[i].c_str());
        std::stringstream message;
        if(p.ping_for_success(0.3, 1) == 0) {
            message << "ping success! "
                    << "ip: " << p.get_addr() << ", "
                    << "seq: " << p.get_seq() << ", "
                    << "time: " << (pinger::get_timestamp() - p.get_sending_ts()) * 1000 << "ms";
        } else {
            message << "ping fail! "
                    << "ip: " << ips[i];
        }
        my_ui.showMessage(message.str());
        my_ui.showProcess((double)(i + 1) / (double)ips.size());
    }
    my_ui.showSuccesses();
}

void simulate_hard_computation() {
    std::this_thread::sleep_for(std::chrono::milliseconds(2000 + rnd()));
}

void finish_scan() {
    return;
}
