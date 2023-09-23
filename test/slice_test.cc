#include <iostream>
#include <vector>

#include "helpers.hh"
using namespace std;

int main(int argc, char *argv[]) {
  vector<uint16_t> vec;
  vector<vector<uint16_t>> vecc;
  for (int i = 0; i < 234; i++) {
    vec.push_back(i);
  }
  vecc = vector_slice(vec, 10);
  return 0;
}
