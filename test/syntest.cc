#include <iostream>

#include "tcp_syn.hh"

int main(int argc, char *argv[])
{
    std::cout << tcp_syn("39.104.55.143", 21) << std::endl;
    return 0;
}