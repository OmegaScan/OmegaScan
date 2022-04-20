#ifndef BASIC_CONFIG
#define BASIC_CONFIG

#include <string>

enum scan_type {
    UDP_SCAN = 1,
    TCP_SYN_SCAN = 1 << 1,
    TCP_CNN_SCAN = 1 << 2,
    TCP_ACK_SCAN = 1 << 3,
};

enum error_type {
    TARGET_RRASE_FAILED = 1,
    PORT_OUT_OF_RANGE = 2,
};

#endif
