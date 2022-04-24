#include "udp_scanner.hh"

int main(int argc, char const *argv[]) {

    std::string host = "39.104.55.143";
    unsigned short port = 8899;
    int ret = udp_scanner(host, port);

    if(-1 == ret) {
        std::cout << "port " << port << " is open/filtered" << std::endl;
    } else if(1 == ret) {
        std::cout << "port " << port << " is closed" << std::endl;
    }

    return 0;
}
