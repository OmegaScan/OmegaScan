#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>

#include "tcp_syn.hh"
#include "helpers.hh"
#include "basic.hh"

int tcp_syn(std::string host, unsigned short port) {
    /* For debug */
    std::cout << "tcp_syn:" << std::endl;
    std::cout << "\thost: " << host << std::endl;
    std::cout << "\tport: " << port << std::endl;
    std::cout << std::endl;

    /* Set up the destination address struct */
    struct sockaddr_in target_addr;
    struct sockaddr_in local_addr;
    memset(&target_addr, 0, sizeof(sockaddr_in));
    memset(&local_addr, 0, sizeof(sockaddr_in));

    /* The validity of host and port will be check there */
    int ret = get_sockaddr(host, port, &target_addr);
    if (ret != 0)
        return ret;

    ret = get_sockaddr(get_local_ip(), port, &local_addr);
    if (ret != 0)
        return ret;

    /* Set up raw socket */
    int sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sock_fd == -1)
        return error_type::SOCKET_CREATE_ERROR;

    /* Tell the system that we are providing the IP header */
    int opt_val = 1;
    if (setsockopt(sock_fd, IPPROTO_IP, IP_HDRINCL, &opt_val, sizeof(int)) < 0) {
        perror_exit("[#] Unable to set IP_HDRINCL socket option\n");
    } else {
        printf("[*] Succesfully set IP_HDRINCL option\n");
    }

    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(int)) < 0) {
        perror_exit("[#] Unable to set SO_REUSEADDR socket option\n");
    } else {
        printf("[*] Succesfully set SO_REUSEADDR option\n");
    }

    /* Set up IP & TCP header */
    struct tcphdr tcp_header;
    struct iphdr ip_header;
    set_ip_hdr(&ip_header, host);
    set_tcp_hdr(&tcp_header, port, LOCAL_PORT, TH_SYN);

    /* Begin to send package */
    ip_header.check = checksum(&ip_header, sizeof(iphdr));
    tcp_header.check = tcp_checksum(&ip_header, &tcp_header);

    char send_buf[MAX_PACKET_LENTH] = {0};
    memcpy(send_buf, &ip_header, sizeof(iphdr));
    memcpy(send_buf + sizeof(iphdr), &tcp_header, sizeof(tcphdr));

    int send_ret = sendto(sock_fd, send_buf, ip_header.tot_len, 0, (struct sockaddr*)&target_addr, sizeof(target_addr));
    if (send_ret <= 0)
        return error_type::SOCKET_SEND_ERROR;

    /* Begin to receive package */
    struct timeval timeout = { 10, 0 };
    if (setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(struct timeval)) < 0) {
        perror_exit("[#] Unable to set SO_RCVTIMEO socket option\n");
    } else {
        printf("[*] Succesfully set SO_RCVTIMEO option\n");
    }

    // Holds the destination network information
    struct sockaddr_storage from_addr;
    socklen_t from_len = 0;
    char recv_buf[MAX_PACKET_LENTH] = {0};
    int recv_ret = recvfrom(sock_fd, recv_buf, MAX_PACKET_LENTH, 0, (struct sockaddr*)&from_addr, &from_len);
    if (recv_ret <= 0)
        return error_type::SOCKET_RECV_ERROR;

    std::cout << "recv: " << (recv_buf) << std::endl;

    /* Begin to receive package */

    return 0;
}
