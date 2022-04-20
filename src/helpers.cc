#include "helpers.hh"

int get_sockaddr(std::string host, unsigned short port, struct sockaddr_in* addr_p) {
    if (port < 1 || port > 65535)
        return error_type::PORT_OUT_OF_RANGE;
    struct sockaddr_in addr = *addr_p;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_aton(host.c_str(), &(addr.sin_addr)) == 0) {
        return error_type::TARGET_RRASE_FAILED;
    };
    return 0;
}
