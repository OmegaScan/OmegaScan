#include "udp_scanner.hh"

int main(int argc, char const *argv[]) {
    std::string host = "39.104.55.143";
    std::vector<unsigned short> ports = {8890, 8894, 8899, 8900, 9000};
    for (unsigned short port : ports) {
        int ret = udp_scanner(host, port);
        if (-1 == ret) {
            std::cout << "port " << port << " is open/filtered" << std::endl;
        }
        else if (1 == ret) {
            std::cout << "port " << port << " is closed" << std::endl;
        }
    }
    return 0;
}
