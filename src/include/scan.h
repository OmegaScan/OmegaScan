#include <vector>
#include <string>

void tcp_syn_scan(std::vector<std::string> ips, std::vector<uint16_t> ports);
void tcp_cnn_scan(std::vector<std::string> ips, std::vector<uint16_t> ports);
void tcp_ack_scan(std::vector<std::string> ips, std::vector<uint16_t> ports);
void udp_scan(std::vector<std::string> ips, std::vector<uint16_t> ports);
void ping_sweep(std::vector<std::string> ips);