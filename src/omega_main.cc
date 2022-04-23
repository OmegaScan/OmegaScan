#include "omega_main.hh"
#include "tcp_syn.hh"

void omega_main(std::string raw_target_host, std::string raw_port_specified, unsigned int raw_options) {
    std::cout << std::endl;
    std::cout << "omega_main:" << std::endl;
    std::cout << "\ttarget_host: " << raw_target_host << std::endl;
    std::cout << "\tport_specified: " << raw_port_specified << std::endl;
    std::cout << "\toptions: " << raw_options << std::endl;
    std::cout << std::endl;

    std::vector<std::string> vs = parse_host("127.0.0.1");
    for(std::string s : vs) {
        std::cout << s << std::endl;
    }

    tcp_syn(raw_target_host, 80);

    return;
}

std::vector<std::string> parse_host(const std::string &raw_target_host) {
    std::vector<std::string> host_list = {};
    std::string input_buffer;
    uint32_t uint32_host = 0, start_host = 0, end_host = 0;
    int cycle_num = 0;
    const char delimiter = '-';

    std::stringstream host_range_str(raw_target_host);
    while (++cycle_num < 3 && std::getline(host_range_str, input_buffer, delimiter)) {
        char *ch_host = (char *) input_buffer.data();
        bool is_valid = false;
        if (inet_addr(ch_host) != INADDR_NONE) {
            uint32_host = ntohl(inet_addr(ch_host));
            is_valid = true;
        }
        if (is_valid && cycle_num == 1)
            start_host = uint32_host;
        else if (is_valid && cycle_num == 2) {
            end_host = uint32_host;
        } else {
            break;
        }
    }
    if (!start_host) {
        std::cout << "The format of ip addresses is invalid." << std::endl;
        return {};
    } else if (!end_host) {
        return {input_buffer};
    } else {
        if (end_host < start_host) {
            std::cout << "The range given is invalid." << std::endl;
            return {};
        } else {
            for (uint32_t u_item = start_host; u_item <= end_host; ++u_item) {
                std::string str_item;
                uint32_t copied_item = u_item;
                std::vector<std::string> stack;
                for (int i = 0; i < 4; ++i) {
                    uint8_t t = copied_item % 256;
                    stack.push_back(std::to_string(t));
                    copied_item /= 256;
                }
                for (int i = 3; 0 < i; --i) {
                    str_item.append(stack[i]);
                    str_item.append(".");
                }
                str_item.append(stack[0]);
                host_list.push_back(str_item);
            }
        }
    }
    return host_list;
}