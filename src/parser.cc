#include "parser.hh"

std::vector<std::string> parse_host(const std::string &raw_target_host) {
    std::vector<std::string> host_list = {};
    std::string str_start_host = raw_target_host, str_end_host = "-1";
    uint32_t start_host, end_host;

    u_int8_t index = 0, input_len = raw_target_host.size();
    while (index < input_len && delimiter != raw_target_host[index++]);
    if (index < 7) {
        return {};
    } else if (index == input_len) {
        if (delimiter == raw_target_host[index - 1]) {
            str_start_host = raw_target_host.substr(0, index - 1);
            str_end_host = LAST_HOST;
        }
    } else {
        str_start_host = raw_target_host.substr(0, index - 1);
        str_end_host = raw_target_host.substr(index, input_len - index);
    }
    start_host = ntohl(inet_addr((char *) str_start_host.data()));
    end_host = ntohl(inet_addr((char *) str_end_host.data()));

    if (start_host == INADDR_NONE && str_start_host != LAST_HOST) {
        std::cout << "The format of ip addresses is invalid." << std::endl;
        return {};
    } else if (end_host == INADDR_NONE && str_end_host != LAST_HOST) {
        if (str_end_host == "-1")host_list.push_back(str_start_host);
        else return {};
    } else {
        if (end_host < start_host) {
            std::cout << "The range given is invalid." << std::endl;
            return {};
        } else {
            host_list.push_back(str_start_host);
            for (uint32_t u_item = start_host + 1; u_item <= end_host && u_item != 0; ++u_item) {
                struct in_addr address = {htonl(u_item)};
                std::string str_item = inet_ntoa(address);
                host_list.push_back(str_item);
            }
        }
    }
    return host_list;
}

std::vector<uint16_t> parse_port(const std::string &raw_port_specified) {
    std::vector<u_int16_t> port_list = {};
    std::string str_start_port = raw_port_specified, str_end_port = "-1";
    u_int16_t start_port, end_port;
    u_int8_t index = 0, input_len = raw_port_specified.size();
    while (index < input_len && delimiter != raw_port_specified[index++]);
    if (index == input_len) {
        if (delimiter == raw_port_specified[index - 1]) {
            str_start_port = raw_port_specified.substr(0, index - 1);
            str_end_port = LAST_PORT;
        }
    } else {
        str_start_port = raw_port_specified.substr(0, index - 1);
        str_end_port = raw_port_specified.substr(index, input_len - index);
    }
    start_port = std::stoi(str_start_port);
    end_port = std::stoi(str_end_port);

    if (start_port == std::stoi(LAST_PORT) && str_start_port != LAST_PORT) {
        std::cout << "The format of ip addresses is invalid." << std::endl;
        return {};
    } else if (end_port == std::stoi(LAST_PORT) && str_end_port != LAST_PORT) {
        if (str_end_port == "-1") port_list.push_back(start_port);
        else return {};
    } else {
        if (end_port < start_port) {
            std::cout << "The range given is invalid." << std::endl;
            return {};
        } else {
            port_list.push_back(start_port);
            for (uint16_t u_item = start_port + 1; u_item <= end_port && u_item != 0; ++u_item) {
                port_list.push_back(u_item);
            }
        }
    }
    return port_list;
}