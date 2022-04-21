#include "pinger.h"

pinger::pinger(const char *ip) {
    seq = 1;
    ident = getpid();

    init_address(ip);
    init_socket();
    init_socket_timeout(0, 100000);
    init_icmp_request();
}

void pinger::init_address(const char *ip) {
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = 0;
    if (inet_aton(ip, (struct in_addr*)&addr.sin_addr.s_addr) == 0) {
        throw "Oops! A invalid ip input.";
    }
}

void pinger::init_icmp_request() {
    bzero(&icmp_request, sizeof(icmp_request));
    icmp_request.type = 8;
    icmp_request.code = 0;
    icmp_request.ident = htons(ident);
    icmp_request.seq = htons(seq);
    strncpy(icmp_request.magic, MAGIC, MAGIC_LEN);
}

void pinger::init_socket() {
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd == -1) {
        throw "Oops! Pinger cannot create a raw socket.";
    }
}

void pinger::init_socket_timeout(time_t sec, time_t usec) {
    // 设置了超时选项（SO_RCVTIMEO）的socket，会成为nonblocking（即使之前是blocking）！！！
    struct timeval tv;
    tv.tv_sec = sec;
    tv.tv_usec = usec;
    int ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    if (ret == -1) {
        throw "Oops! Pinger cannot set the timeout option of socket.";
    }
}

pinger::~pinger() {
    close(sockfd);
}

double get_timestamp() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + ((double)tv.tv_usec) / 1000000;
}

uint16_t pinger::calculate_checksum(unsigned char* buffer, int bytes) {
    uint32_t checksum = 0;
    unsigned char* end = buffer + bytes;

    // odd bytes add last byte and reset end
    if (bytes % 2 == 1) {
        end = buffer + bytes - 1;
        checksum += (*end) << 8;
    }

    // add words of two bytes, one by one
    while (buffer < end) {
        checksum += buffer[0] << 8;
        checksum += buffer[1];
        buffer += 2;
    }

    // add carry if any
    uint32_t carray = checksum >> 16;
    while (carray) {
        checksum = (checksum & 0xffff) + carray;
        carray = checksum >> 16;
    }

    // negate it
    checksum = ~checksum;

    return checksum & 0xffff;
}

int pinger::send_icmp_request() {

    icmp_request.sending_ts = get_timestamp();
    icmp_request.checksum = htons(
                                calculate_checksum((unsigned char*)&icmp_request, sizeof(icmp_request))
                            );

    int bytes = sendto(sockfd, &icmp_request, sizeof(icmp_request), 0,
                       (struct sockaddr*)&addr, sizeof(addr));
    if (bytes == -1) {
        throw "Oops! socket::sendto fail.";
    }

    this->seq += 1;

    return 0;
}

int pinger::recv_icmp_reply() {

    char buffer[MTU];
    struct sockaddr_in dest_addr; // 用于接收源地址

    unsigned int addr_len = sizeof(dest_addr);
    int bytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&dest_addr, &addr_len);
    if (bytes == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return ICMP_NO_RECV;
        }
        throw "Oops! socket::recvfrom fail.";
    }

    memcpy(&icmp_reply, buffer + 20, sizeof(icmp_echo));

    if (icmp_reply.type != 0 || icmp_reply.code != 0) {
        return ICMP_UNEXCEPT_TYPE_RECV;
    }

    if (ntohs(icmp_reply.ident) != ident) {
        return ICMP_UNEXCEPT_IDENT_RECV;
    }

    return ICMP_CORRECT_RECV;
}

int pinger::ping_for_success(double send_gap_time, double timeout) {

    double start_time = get_timestamp();
    double next_send_time = get_timestamp();

    while (true) {

        if (get_timestamp() >= next_send_time) {
            send_icmp_request();
            next_send_time += send_gap_time;
        }

        if (recv_icmp_reply() == ICMP_CORRECT_RECV) {
            return 0;
        }

        if (get_timestamp() - start_time > timeout) {
            return -1;
        }
    }
}

int pinger::ping(double timeout) {

    double start_time = get_timestamp();

    send_icmp_request();

    while (true) {

        if (recv_icmp_reply() == ICMP_CORRECT_RECV) {
            return 0;
        }

        if (get_timestamp() - start_time > timeout) {
            return -1;
        }
    }
}

int pinger::get_seq() {
    return ntohs(icmp_reply.seq);
}

char* pinger::get_addr() {
    return inet_ntoa(addr.sin_addr);
}

double pinger::get_sending_ts() {
    return icmp_request.sending_ts;
}

#include <stdio.h>
void pinger_exampale() {

    pinger p("8.8.8.8");

    int ret = p.ping_for_success(1, 5);

    if (ret == 0) {
        printf("%s seq=%d %5.2fms\n",
               p.get_addr(),
               p.get_seq(),
               (get_timestamp() - p.get_sending_ts()) * 1000
              );
    }

}

int main() {
    pinger_exampale();
}