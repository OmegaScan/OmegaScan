#include <vector>
#include <string>
#include <iostream>
#include <random>
#include <sstream>

#include "tcp_ack.hh"
#include "tcp_scanner.h"
#include "tcp_syn.hh"
#include "udp_scanner.hh"
#include "pinger.h"
#include "ui.hh"
#include "basic.hh"
#include "helpers.hh"
#include "tcp_syn.hh"
#include "threader.hh"
#define THREAD_NUMBER 100


void tcp_syn_scan(std::vector<std::string> ips, std::vector<uint16_t> ports);
void tcp_cnn_scan(std::vector<std::string> ips, std::vector<uint16_t> ports);
void tcp_ack_scan(std::vector<std::string> ips, std::vector<uint16_t> ports);
void udp_scan(std::vector<std::string> ips, std::vector<uint16_t> ports);
void ping_sweep(std::vector<std::string> ips);
void simulate_hard_computation();
void finish_scan();
