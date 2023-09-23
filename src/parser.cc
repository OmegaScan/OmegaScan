#include "parser.hh"

using namespace std;

vector<string> parse_host(const string &raw_host) {
  vector<string> host_list = {};
  string str_start_host = raw_host, str_end_host = "-1";
  uint32_t start_host, end_host;

  u_int8_t index = 0, input_len = raw_host.size();
  while (index < input_len && delimiter != raw_host[index++])
    ;
  if (index < 7)
    return {};
  else if (index == input_len) {
    if (delimiter == raw_host[index - 1]) {
      str_start_host = raw_host.substr(0, index - 1);
      str_end_host = LAST_HOST;
    }
  } else {
    str_start_host = raw_host.substr(0, index - 1);
    str_end_host = raw_host.substr(index, input_len - index);
  }
  start_host = ntohl(inet_addr((char *)str_start_host.data()));
  end_host = ntohl(inet_addr((char *)str_end_host.data()));

  if (start_host == INADDR_NONE && str_start_host != LAST_HOST)
    return {};
  else if (end_host == INADDR_NONE && str_end_host != LAST_HOST) {
    if (str_end_host == "-1")
      host_list.push_back(str_start_host);
    else
      return {};
  } else {
    if (end_host < start_host)
      return {};
    else {
      host_list.push_back(str_start_host);
      for (uint32_t u_item = start_host + 1; u_item <= end_host && u_item != 0;
           ++u_item) {
        struct in_addr address = {htonl(u_item)};
        string str_item = inet_ntoa(address);
        host_list.push_back(str_item);
      }
    }
  }
  return host_list;
}

vector<unsigned short> parse_port(const string &raw_port) {
  vector<u_int16_t> port_list = {};
  string str_start_port = raw_port, str_end_port = "-1";
  u_int16_t start_port, end_port;
  u_int8_t index = 0, input_len = raw_port.size();
  while (index < input_len && delimiter != raw_port[index++])
    ;
  if (index == input_len) {
    if (delimiter == raw_port[index - 1]) {
      str_start_port = raw_port.substr(0, index - 1);
      str_end_port = LAST_PORT;
    }
  } else {
    str_start_port = raw_port.substr(0, index - 1);
    str_end_port = raw_port.substr(index, input_len - index);
  }
  start_port = stoi(str_start_port);
  end_port = stoi(str_end_port);

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
      for (unsigned short u_item = start_port + 1;
           u_item <= end_port && u_item != 0; ++u_item) {
        port_list.push_back(u_item);
      }
    }
  }
  return port_list;
}
