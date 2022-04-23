#ifndef BASIC_CONFIG
#define BASIC_CONFIG

#define LOCAL_PORT 30000
#define TCP_SEQ_NUM 0x1234
#define TCP_WIN_SIZE 0x4000
#define CUSTOM_IP_ID 0x1000
#define CUSTOM_IP_TTL 64
#define MAX_PACKET_LENTH 8192

enum scan_type {
    UDP_SCAN = 1,
    TCP_SYN_SCAN = 1 << 1,
    TCP_CNN_SCAN = 1 << 2,
    TCP_ACK_SCAN = 1 << 3,
};

enum error_type {
    TARGET_PRASE_FAILED = 1,
    PORT_OUT_OF_RANGE = 2,
    SOCKET_CREATE_ERROR = 3,
    SOCKET_SEND_ERROR = 4,
    SOCKET_RECV_ERROR = 5,
};

#endif
