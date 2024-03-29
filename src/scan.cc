#include "scan.hh"

using namespace std;

random_device rd;
mt19937 mt(rd());
uniform_int_distribution<int> dist(-1000, 1000);
auto rnd = bind(dist, mt);
ui my_ui;  // 本来不想用全局变量的，但是由于C的回调函数...
pthread_mutex_t mutex_lock = PTHREAD_MUTEX_INITIALIZER;
long countP = 0;

void print_tcp_syn(string host, unsigned short port, int total) {
  int ret = tcp_syn(host, port, LOCAL_PORT);

  pthread_mutex_lock(&mutex_lock);
  my_ui.show_scanning(host, port);

  // 生成扫描结果字符串
  stringstream message;
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
  my_ui.show_message(message.str());

  countP++;

  my_ui.show_process((double)countP / (double)total);
  pthread_mutex_unlock(&mutex_lock);
}

void tcp_syn_scan(vector<string> ips, vector<uint16_t> ports) {
  ThreadPool pool(THREAD_NUMBER);
  pool.init();

  int scanned_count = 0;

  for (int i = 0; i < ips.size(); i++) {
    for (int j = 0; j < ports.size(); j++) {
      double process_value =
          (double)++scanned_count / (double)(ips.size() * ports.size());
      pool.submit(print_tcp_syn, ips[i], ports[j], ips.size() * ports.size());
    }
  }
  auto finish_status = pool.submit(finish_scan);
  finish_status.get();
  pool.shutdown();

  my_ui.show_successes();
}

void tcp_cnn_scan_success_handler(uint32_t ip, uint16_t port, int fd) {
  int p1 = (u_char)((ip >> 24) & 0xFF);
  int p2 = (u_char)((ip >> 16) & 0xFF);
  int p3 = (u_char)((ip >> 8) & 0xFF);
  int p4 = (u_char)(ip & 0xFF);

  stringstream message;
  message << "ip: " << p1 << "." << p2 << "." << p3 << "." << p4 << ", "
          << "port: " << port << " -> open";
  my_ui.show_message(message.str());
}

void tcp_cnn_scan(vector<string> ips, vector<uint16_t> ports) {
  tcp_scanner scanner;
  scanner.set_timeout({1, 0});
  scanner.set_success_callback(tcp_cnn_scan_success_handler);

  int scanned_count = 0;

  for (int i = 0; i < ips.size(); i++) {
    uint32_t ip = ntohl(inet_addr(ips[i].c_str()));
    if (ports.size() <= 300) {
      my_ui.show_scanning(ips[i], ports);
      scanner.scan(ip, ports);
      my_ui.show_process((double)++scanned_count / (double)ips.size());
    } else {
      auto ports_fragments = vector_slice(ports, 300);
      for (auto ports_fragment : ports_fragments) {
        my_ui.show_scanning(ips[i], ports_fragment);
        scanner.scan(ip, ports_fragment);
        scanned_count += ports_fragment.size();
        my_ui.show_process((double)scanned_count /
                           (double)(ips.size() * ports.size()));
      }
    }
  }
  my_ui.show_successes();
}

void print_tcp_ack(string host, unsigned short port, int total) {
  int ret = tcp_syn(host, port, LOCAL_PORT);

  pthread_mutex_lock(&mutex_lock);
  my_ui.show_scanning(host, port);
  stringstream message;
  if (ret > 0) {
    message << "ip: " << host << ", "
            << "port: " << port << " -> unfiltered";
  } else {
    message << "ip: " << host << ", "
            << "port: " << port << " -> filtered";
  }
  my_ui.show_message(message.str());
  countP++;

  my_ui.show_process((double)countP / (double)total);
  pthread_mutex_unlock(&mutex_lock);
}

void tcp_ack_scan(vector<string> ips, vector<uint16_t> ports) {
  ThreadPool pool(THREAD_NUMBER);

  pool.init();

  int scanned_count = 0;

  for (int i = 0; i < ips.size(); i++) {
    for (int j = 0; j < ports.size(); j++) {
      // double process_value = (double)++scanned_count / (double)(ips.size() *
      // ports.size());
      pool.submit(print_tcp_ack, ips[i], ports[j], ips.size() * ports.size());
    }
  }
  auto finish_status = pool.submit(finish_scan);
  finish_status.get();
  pool.shutdown();
  my_ui.show_successes();
}

void udp_scan(vector<string> ips, vector<uint16_t> ports) {
  int scanned_count = 0;

  for (int i = 0; i < ips.size(); i++) {
    for (int j = 0; j < ports.size(); j++) {
      my_ui.show_scanning(ips[i], ports[j]);
      int ret = udp_scanner(ips[i], ports[j]);

      stringstream message;
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
      my_ui.show_message(message.str());
      my_ui.show_process((double)++scanned_count /
                         (double)(ips.size() * ports.size()));
    }
  }
  my_ui.show_successes();
}

void ping_sweep(vector<string> ips) {
  for (int i = 0; i < ips.size(); i++) {
    my_ui.show_scanning(ips[i], 0);
    pinger p(ips[i].c_str());
    stringstream message;
    if (p.ping_for_success(0.3, 1) == 0) {
      message << "ping success! "
              << "ip: " << p.get_addr() << ", "
              << "seq: " << p.get_seq() << ", "
              << "time: "
              << (pinger::get_timestamp() - p.get_sending_ts()) * 1000 << "ms";
    } else {
      message << "ping fail! "
              << "ip: " << ips[i];
    }
    my_ui.show_message(message.str());
    my_ui.show_process((double)(i + 1) / (double)ips.size());
  }
  my_ui.show_successes();
}

void simulate_hard_computation() {
  this_thread::sleep_for(chrono::milliseconds(2000 + rnd()));
}

void finish_scan() { return; }
