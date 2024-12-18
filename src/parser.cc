#include "parser.hh"

using namespace std;

bool is_valid_ip(const string &ip) {
  struct sockaddr_in sa;
  return inet_pton(AF_INET, ip.c_str(), &(sa.sin_addr)) != 0;
}

vector<string> parse_host(const string &raw_host) {
  vector<string> host_list;

  if (raw_host.empty()) {
    return host_list;
  }

  string str_start_host = raw_host, str_end_host = LAST_HOST;

  size_t delimiter_pos = raw_host.find(delimiter);
  if (delimiter_pos != string::npos) {
    str_start_host = raw_host.substr(0, delimiter_pos);
    str_end_host = raw_host.substr(delimiter_pos + 1);
  }

  if (str_end_host == LAST_HOST) {
    if (is_valid_ip(str_start_host)) {
      host_list.push_back(str_start_host);
    }
    return host_list;
  }

  struct in_addr start_addr, end_addr;
  if (!is_valid_ip(str_start_host) || !is_valid_ip(str_end_host)) {
    return {};
  }

  inet_pton(AF_INET, str_start_host.c_str(), &start_addr);
  inet_pton(AF_INET, str_end_host.c_str(), &end_addr);

  uint32_t start_ip = ntohl(start_addr.s_addr);
  uint32_t end_ip = ntohl(end_addr.s_addr);

  if (start_ip > end_ip) {
    return {};
  }

  for (uint32_t ip = start_ip; ip <= end_ip; ++ip) {
    struct in_addr addr;
    addr.s_addr = htonl(ip);
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr, ip_str, INET_ADDRSTRLEN);
    host_list.push_back(ip_str);
  }

  return host_list;
}

vector<u_int16_t> parse_port(const string &raw_port) {
  vector<u_int16_t> port_list;

  if (raw_port.empty()) {
    return port_list;
  }

  string str_start_port = raw_port, str_end_port = "-1";
  u_int16_t start_port, end_port;
  u_int8_t index = 0, input_len = raw_port.size();

  while (index < input_len && delimiter != raw_port[index++]);

  if (index == input_len) {
    if (delimiter == raw_port[index - 1]) {
      str_start_port = raw_port.substr(0, index - 1);
      str_end_port = LAST_PORT;
    }
  } else {
    str_start_port = raw_port.substr(0, index - 1);
    str_end_port = raw_port.substr(index, input_len - index);
  }

  if (str_start_port.empty()) {
    return {};
  }

  try {
    start_port = stoi(str_start_port);
    end_port = stoi(str_end_port);
  } catch (const std::invalid_argument &e) {
    return {};
  }

  if (start_port == stoi(LAST_PORT) && str_start_port != LAST_PORT)
    return {};
  else if (end_port == stoi(LAST_PORT) && str_end_port != LAST_PORT) {
    if (str_end_port == "-1")
      port_list.push_back(start_port);
    else
      return {};
  } else {
    if (end_port < start_port)
      return {};
    else {
      port_list.push_back(start_port);
      for (unsigned short u_item = start_port + 1; u_item <= end_port; ++u_item) {
        port_list.push_back(u_item);
      }
    }
  }

  return port_list;
}
