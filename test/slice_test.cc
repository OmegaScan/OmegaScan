#include <iostream>
#include <vector>
#include "helpers.hh"

int main(int argc, char *argv[])
{
    std::vector<int> vec;
    std::vector<std::vector<int>> vecc;
    for (int i = 0; i < 234; i++) {
        vec.push_back(i);
    }
    vecc = vector_slice(vec, 10);
    return 0;
}
