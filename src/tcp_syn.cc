#include "tcp_syn.hh"

int tcp_syn(std::string host, unsigned short port) {
    struct sockaddr_in addr;
    int ret = get_sockaddr(host, port, &addr);
    if (ret != 0)
        return ret;

    std::cout << "tcp_syn:" << std::endl;
    std::cout << "\thost: " << host << std::endl;
    std::cout << "\tport: " << port << std::endl;
    std::cout << "\taddr: " << addr.sin_addr.s_addr << std::endl;
    std::cout << std::endl;

    return 0;
}
