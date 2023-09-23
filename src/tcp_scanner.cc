#include "tcp_scanner.hh"

using namespace std;

tcp_scanner::tcp_scanner() { timeout = {1, 0}; }

tcp_scanner::~tcp_scanner() {}

void tcp_scanner::close_all_fd() {
  for (int &sockfd : sockfds) {
    close(sockfd);
    sockfd = -1;
  }
}

int tcp_scanner::create_sockets() {
  sockfds.clear();
  sockfds.resize(ports.size());

  int success_count = 0;

  for (int i = 0; i < ports.size(); i++) {
    int sockfd = -1;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      throw "Oops! open socket fail.";
    }
    if (fcntl(sockfd, F_SETFL, O_NDELAY) < 0) { /* 设置为非阻塞式socket */
      throw "Oops! fcntl() error, cannot set socket to unblock.";
    }
    sockfds[i] = sockfd;
    success_count++;
  }
  return success_count;
}

void tcp_scanner::connect_sockets() {
  for (int i = 0; i < sockfds.size(); i++) {
    struct sockaddr_in dest;
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = htonl(addr);
    dest.sin_port = htons(ports[i]);

    connect(sockfds[i], (struct sockaddr *)&dest, sizeof(dest));
  }
}

void tcp_scanner::set_timeout(timeval timeout) { this->timeout = timeout; }

int tcp_scanner::wait_connection_success() {
  // 初始化关注的描述符集
  FD_ZERO(&fdset);
  for (int sockfd : sockfds) {
    if (sockfd < 0) {
      continue;
    }
    FD_SET(sockfd, &fdset);
  }

  struct timeval timeout;
  timeout.tv_sec = this->timeout.tv_sec;
  timeout.tv_usec = this->timeout.tv_usec;

  int maxfd = *max_element(sockfds.begin(), sockfds.end());

  return select(maxfd + 1, (fd_set *)NULL, &fdset, (fd_set *)NULL, &timeout);
}

vector<int> tcp_scanner::get_success_socks_idx() {
  vector<int> result;

  for (int i = 0; i < sockfds.size(); i++) {
    if (sockfds[i] == -1) {
      continue;
    }
    if (!FD_ISSET(sockfds[i], &fdset)) {
      continue;
    }

    // 判断该socket是否处于错误状态
    int err = 1;
    int errlen = 1;
    getsockopt(sockfds[i], SOL_SOCKET, SO_ERROR, (char *)&err,
               (socklen_t *)&errlen);
    if (err != 0) {
      close(sockfds[i]);
      sockfds[i] = -1;
      continue;
    }

    result.push_back(i);
  }
  return result;
}

void tcp_scanner::scan(uint32_t addr, const vector<uint16_t> &ports) {
  this->addr = addr;
  this->ports = ports;

  create_sockets();
  connect_sockets();

  while (true) {
    // 等待成功的连接
    switch (wait_connection_success()) {
      case -1:
        close_all_fd();
        throw "Oops! select() error";
        break;
      case 0: /* 超时 */
        close_all_fd();
        return;
      default:
        break;
    }

    // 获取成功的连接并处理
    for (int i : get_success_socks_idx()) {
      success_callback(this->addr, this->ports[i], this->sockfds[i]);
      close(sockfds[i]);
      sockfds[i] = -1;
    }
  }
}

void tcp_scanner::set_success_callback(void (*f)(uint32_t, uint16_t, int)) {
  this->success_callback = f;
}
