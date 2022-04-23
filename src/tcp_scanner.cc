#include "tcp_scanner.h"

tcpScanner::tcpScanner() {
    timeout = {1, 0};
}

tcpScanner::~tcpScanner() {
}

void tcpScanner::closeAllFd() {
    for (int& sockfd : sockfds) {
        close(sockfd);
        sockfd = -1;
    }
}

int tcpScanner::createSockets() {

    sockfds.clear();
    sockfds.resize(ports.size());

    int success_count = 0;

    for (int i = 0; i < ports.size(); i++) {
        int sockfd = -1;
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            throw "Oops! open socket fail.";
        }
        if(fcntl(sockfd, F_SETFL, O_NDELAY) < 0) { /* 设置为非阻塞式socket */
            throw "Oops! fcntl() error, cannot set socket to unblock.";
        }
        sockfds[i] = sockfd;
        success_count++;
    }
    return success_count;
}

void tcpScanner::connectSockets() {

    for (int i = 0; i < sockfds.size(); i++) {
        struct sockaddr_in dest;
        dest.sin_family = AF_INET;
        dest.sin_addr.s_addr = htonl(addr);
        dest.sin_port = htons(ports[i]);

        connect(sockfds[i], (struct sockaddr *)&dest, sizeof(dest));
    }
}

void tcpScanner::setTimeout(timeval timeout) {
    this->timeout = timeout;
}

int tcpScanner::waitConnectionSuccess() {

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

    return select(maxfd + 1, (fd_set*)NULL, &fdset, (fd_set*)NULL, &timeout);
}

std::vector<int> tcpScanner::getSuccessSocksIndex() {
    std::vector<int> result;

    for(int i = 0; i < sockfds.size(); i++) {
        if (sockfds[i] == -1) {
            continue;
        }
        if(!FD_ISSET(sockfds[i], &fdset)) {
            continue;
        }

        // 判断该socket是否处于错误状态
        int err = 1;
        int errlen = 1;
        getsockopt(sockfds[i], SOL_SOCKET, SO_ERROR, (char*)&err, (socklen_t *)&errlen);
        if (err != 0)  {
            close(sockfds[i]);
            sockfds[i] = -1;
            continue;
        }

        result.push_back(i);
    }
    return result;
}

void tcpScanner::scan(u_long addr, const std::vector<int>& ports) {
    this->addr = addr;
    this->ports = ports;

    createSockets();
    connectSockets();

    while (true) {

        // 等待成功的连接
        switch (waitConnectionSuccess()) {
        case -1:
            closeAllFd();
            throw "Oops! select() error";
            break;
        case 0: /* 超时 */
            closeAllFd();
            return;
        default:
            break;
        }

        // 获取成功的连接并处理
        for(int i : getSuccessSocksIndex()) {
            successCallback(this->addr, this->ports[i], this->sockfds[i]);
            close(sockfds[i]);
            sockfds[i] = -1;
        }
    }
}

void tcpScanner::setSuccessCallback(void (*f)(u_long, int, int)) {
    this->successCallback = f;
}
