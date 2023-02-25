#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "basic.hh"
#include "helpers.hh"
#include "pinger.hh"
#include "tcp_ack.hh"
#include "tcp_scanner.hh"
#include "tcp_syn.hh"
#include "threader.hh"
#include "udp_scanner.hh"
#include "ui.hh"
#define THREAD_NUMBER 100

void tcp_syn_scan(std::vector<std::string> ips, std::vector<uint16_t> ports);
void tcp_cnn_scan(std::vector<std::string> ips, std::vector<uint16_t> ports);
void tcp_ack_scan(std::vector<std::string> ips, std::vector<uint16_t> ports);
void udp_scan(std::vector<std::string> ips, std::vector<uint16_t> ports);
void ping_sweep(std::vector<std::string> ips);
void simulate_hard_computation();
void finish_scan();
