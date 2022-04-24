#include "udp_scanner.hh"

int udp_scanner(std::string raw_target_host, std::vector<unsigned short> port_list) {

    if (!port_list.empty()) {
        char buffer[1024];
        char *ch_host = (char *) raw_target_host.data();
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

        for (unsigned short us_port : port_list) {

            socklen_t target_len = sizeof(addr_target);
            socklen_t source_len = sizeof(addr_source);

            addr_target.sin_port = htons(us_port);
            addr_source.sin_port = 0;

            sendto(sockfd, buffer, 1024, 0, (struct sockaddr *) &addr_target, target_len);

            struct timeval timeout = {3, 0};
            setsockopt(sock_recvfd, SOL_SOCKET, SO_RCVTIMEO, (const char *) &timeout, sizeof(timeout));

            if (recvfrom(sock_recvfd, buffer, 1024, 0, (struct sockaddr *) &addr_source, &source_len) < 0) {
                std::cout << us_port << " is open/filtered." << std::endl;
                continue;
            } else {
                std::cout << us_port << " is closed." << std::endl;
            }
        }
        close(sockfd);
        close(sock_recvfd);
        return 1;
    }
    return -1;
}