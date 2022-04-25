#include <iostream>
#include <unistd.h>

#include "tcp_syn.hh"

int main(int argc, char *argv[])
{
    std::cout << "21 port: " << tcp_syn("39.104.55.143", 21) << std::endl;
    sleep(1);
    std::cout << "22 port: " << tcp_syn("39.104.55.143", 22) << std::endl;
    sleep(1);
    std::cout << "80 port: " << tcp_syn("39.104.55.143", 80) << std::endl;
    sleep(1);
    std::cout << "443 port: " << tcp_syn("39.104.55.143", 443) << std::endl;
    sleep(1);
    std::cout << "1234 port: " << tcp_syn("39.104.55.143", 1234) << std::endl;
    return 0;
}
