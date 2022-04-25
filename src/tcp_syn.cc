#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>

#include "tcp_syn.hh"
#include "helpers.hh"
#include "basic.hh"

// static unsigned short local_port = LOCAL_PORT;

int tcp_syn(std::string host, unsigned short port, unsigned short local_port) {
    /* Set up raw socket */
    int sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sock_fd == -1)
        return error_type::SOCKET_CREATE_ERROR;

    /* Tell the system that we are providing the IP header */
    int opt_val = 1;
    if (setsockopt(sock_fd, IPPROTO_IP, IP_HDRINCL, &opt_val, sizeof(int)) < 0) {
        perror_exit("[#] Unable to set IP_HDRINCL socket option\n");
    }

    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(int)) < 0) {
        perror_exit("[#] Unable to set SO_REUSEADDR socket option\n");
    }

    /* Set up IP & TCP header */
    struct tcphdr tcp_header;
    struct iphdr ip_header;
    set_ip_hdr(&ip_header, host);
    set_tcp_hdr(&tcp_header, port, local_port, TH_SYN);

    /* Set up the destination address struct */
    struct sockaddr_in target_addr;
    struct sockaddr_in local_addr;
    memset(&target_addr, 0, sizeof(sockaddr_in));
    memset(&local_addr, 0, sizeof(sockaddr_in));

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

    char send_buf[MAX_PACKET_LENTH] = {0};
    memcpy(send_buf, &ip_header, sizeof(iphdr));
    memcpy(send_buf + sizeof(iphdr), &tcp_header, sizeof(tcphdr));

    int send_ret = sendto(sock_fd, send_buf, ip_header.tot_len, 0, (struct sockaddr*)&target_addr, sizeof(target_addr));
    if (send_ret <= 0)
        return error_type::SOCKET_SEND_ERROR;

    /* Begin to receive package */
    // Set receive timeout
    struct timeval timeout = { 5, 0 };
    if (setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(struct timeval)) < 0) {
        perror_exit("[#] Unable to set SO_RCVTIMEO socket option\n");
    }

    // Holds the destination network information
    struct sockaddr_storage from_addr;
    socklen_t from_len = 0;
    char recv_buf[MAX_PACKET_LENTH];
    int recv_ret;
    struct iphdr *recv_iph;
    struct tcphdr *recv_tcph;
    unsigned retry = 0;
    do {
        memset(recv_buf, 0, MAX_PACKET_LENTH);
        recv_ret= recvfrom(sock_fd, recv_buf, MAX_PACKET_LENTH, 0, (struct sockaddr*)&from_addr, &from_len);
        recv_iph = (struct iphdr*)recv_buf;
        recv_tcph = (struct tcphdr*)(recv_buf + 4 * (recv_iph->ihl));
    } while (retry++ < RETRY_TIMES && recv_ret != 1 && (ntohs(recv_tcph->th_sport) != ntohs(tcp_header.th_dport) || ntohl(recv_iph->saddr) != ntohl(ip_header.daddr)));
    // For resource saving
    close(sock_fd);
    if (recv_ret <= 0 || retry > RETRY_TIMES)
        return error_type::SOCKET_RECV_ERROR;


    debug(print_hdr_msg(recv_buf))

    unsigned int flags = get_flag_of(recv_buf, sizeof(iphdr) + sizeof(tcphdr));
    if ((flags >> 2) % 2 == 1) {
        debug(std::cout << "RST flag is on!" << std::endl)
        return syn_res::RST;
    } else if ((flags >> 4) % 2 == 1) {
        debug(std::cout << "ACK flag is on!" << std::endl)
        return syn_res::ACK;
    } else {
        debug(std::cout << "Unkown flags" << std::endl)
        debug(std::cout << "Flags: " << flags << std::endl)
        return error_type::UNKNOWN_RESULT;
    }

    return 0;
}
