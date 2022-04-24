#include "udp_scanner.hh"

int udp_scanner(std::string host, unsigned short port) {
    char buffer[20];
    char *ch_host = (char *) host.data();
    struct sockaddr_in addr_target, addr_source;
    addr_target.sin_family = AF_INET;
    addr_source.sin_family = AF_INET;

    addr_target.sin_addr.s_addr = inet_addr(ch_host);
    addr_source.sin_addr.s_addr = inet_addr(ch_host);

    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    int sock_recvfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    if (-1 == sockfd || -1 == sock_recvfd) {
        perror("socket");
        exit(1);
    }

    socklen_t target_len = sizeof(addr_target), source_len = sizeof(addr_source);

    addr_target.sin_port = htons(port);
    addr_source.sin_port = 0;

    sendto(sockfd, buffer, 20, 0, (struct sockaddr *) &addr_target, target_len);

    struct timeval timeout = {3, 0};
    setsockopt(sock_recvfd, SOL_SOCKET, SO_RCVTIMEO, (const char *) &timeout, sizeof(timeout));

    int status = recvfrom(sock_recvfd, buffer, 20, 0, (struct sockaddr *) &addr_source, &source_len);
    close(sockfd);
    close(sock_recvfd);
    if(status < 0) return -1;
    return 1;
}