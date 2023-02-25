#include <iostream>
#include <random>
#include <vector>

#include "tcp_syn.hh"
#include "threader.hh"
#define THREAD_NUMBER 5

std::random_device rd;

std::mt19937 mt(rd());

std::uniform_int_distribution<int> dist(-1000, 1000);

auto rnd = std::bind(dist, mt);

// 设置线程睡眠时间
void simulate_hard_computation() {
  std::this_thread::sleep_for(std::chrono::milliseconds(2000 + rnd()));
}

void print_status(const unsigned short u) {
  int status = tcp_syn("39.104.55.143", u);
  simulate_hard_computation();
  // std::cout << "port:" << u << "  status:" << status << std::endl;
  printf("port:%u, status:%d\n", u, status);
}
void finished() { return; }

void example() {
  ThreadPool pool(5);

  pool.init();

  for (unsigned short port = 10; port < 30; ++port) {
    pool.submit(print_status, port);
  }
  auto f = pool.submit(finished);
  f.get();

  pool.shutdown();
}

int main() {
  example();
  return 0;
}