#include "omega_main.hh"
#include "tcp_syn.hh"

void omega_main(std::string raw_target_host, std::string raw_port_specified, unsigned int raw_options) {
    std::cout << std::endl;
    std::cout << "omega_main:" << std::endl;
    std::cout << "\ttarget_host: " << raw_target_host << std::endl;
    std::cout << "\tport_specified: " << raw_port_specified << std::endl;
    std::cout << "\toptions: " << raw_options << std::endl;
    std::cout << std::endl;

    // std::vector<std::string> vs = parse_host("127.0.0.1-127.0.1.3");
    // for(std::string s : vs) {
    //     std::cout << s << std::endl;
    // }

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
        bool is_valid = check(input_buffer, uint32_host);
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

bool check(const std::string &str_host, uint32_t &uint32_host) {
    const char dot = '.';
    bool is_valid = false;
    std::string buffer;
    std::stringstream host_str(str_host);
    std::vector<std::string> vec_temp;
    uint32_host = 0;
    while (std::getline(host_str, buffer, dot))
        vec_temp.push_back(buffer);

    for (int i = 0; vec_temp.size() == 4 && i < vec_temp.size(); ++i) {
        uint32_t u = stoi(vec_temp[i]);
        if (255 < u) {
            is_valid = false;
        } else {
            uint32_host += u << (8 * (3 - i));
            is_valid = true;
        }
    }
    return is_valid;
}
