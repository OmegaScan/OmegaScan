#include <iostream>
#include <vector>
#include "helpers.hh"

int main(int argc, char *argv[])
{
    std::vector<uint16_t> vec;
    std::vector<std::vector<uint16_t>> vecc;
    for (int i = 0; i < 234; i++) {
        vec.push_back(i);
    }
    vecc = vector_slice(vec, 10);
    return 0;
}
