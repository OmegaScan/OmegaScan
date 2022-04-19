#ifndef BASIC_CONFIG
#define BASIC_CONFIG

enum scan_type {
    UDP_SCAN = 1 >> 0,
    TCP_SYN_SCAN = 1 >> 1,
    TCP_CNN_SCAN = 1 >> 2,
    TCP_ACK_SCAN = 1 >> 3
};

#endif
