#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>

#include "tcp_ack.hh"
#include "helpers.hh"
#include "basic.hh"

static unsigned short local_port = LOCAL_PORT;

int tcp_syn(std::string host, unsigned short port) {
    /* Set up raw socket */
    int sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sock_fd == -1)
        return error_type::SOCKET_CREATE_ERROR;

    /* Set up IP & TCP header */
    struct tcphdr tcp_header;
    struct iphdr ip_header;
    set_ip_hdr(&ip_header, host);
    set_tcp_hdr(&tcp_header, port, local_port++, TH_ACK);

    /* Set up the destination address struct */
    struct sockaddr_in target_addr;
    struct sockaddr_in local_addr;
    memset(&target_addr, 0, sizeof(sockaddr_in));
    memset(&local_addr, 0, sizeof(sockaddr_in));

    /* Tell the system that we are providing the IP header */
    int opt_val = 1;
    if (setsockopt(sock_fd, IPPROTO_IP, IP_HDRINCL, &opt_val, sizeof(int)) < 0) {
        perror_exit("[#] Unable to set IP_HDRINCL socket option\n");
    }
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(int)) < 0) {
        perror_exit("[#] Unable to set SO_REUSEADDR socket option\n");
    }

    // The validity of host and port will be check there
    int ret = get_sockaddr(host, port, &target_addr);
    if (ret != 0)
        return ret;

    ret = get_sockaddr(get_local_ip(), port, &local_addr);
    if (ret != 0)
        return ret;

    /* Begin to send package */
    // Fill in checksum first
    ip_header.check = checksum(&ip_header, sizeof(iphdr));
    tcp_header.check = tcp_checksum(&ip_header, &tcp_header);

    char BUF_send[MAX_PACKET_LENTH] = {0};
    memcpy(BUF_send, &ip_header, sizeof(iphdr));
    memcpy(BUF_send + sizeof(iphdr), &tcp_header, sizeof(tcphdr));

    int send_ret = sendto(sock_fd, BUF_send, ip_header.tot_len, 0, (struct sockaddr*)&target_addr, sizeof(target_addr));
    if (send_ret <= 0)
        return error_type::SOCKET_SEND_ERROR;

    /* Begin to receive package */
    // Set receive timeout
    struct timeval timeout = { 1, 0 };
    if (setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(struct timeval)) < 0) {
        perror_exit("[#] Unable to set SO_RCVTIMEO socket option\n");
    }

    // Holds the destination network information
    struct sockaddr_storage from_addr;
    socklen_t from_len = 0;
    char BUF_rec[MAX_PACKET_LENTH] = {0};
    int recv_ret = recvfrom(sock_fd, BUF_rec, MAX_PACKET_LENTH, 0, (struct sockaddr*)&from_addr, &from_len);
    // For resource saving
    close(sock_fd);
    if (recv_ret <= 0)
        return error_type::SOCKET_RECV_ERROR;

    debug(print_hdr_msg(BUF_rec))

    int rst = (get_flag_of(BUF_rec, sizeof(iphdr) + sizeof(tcphdr)) >> 2) % 2;
    return rst;
    return 0;
}
