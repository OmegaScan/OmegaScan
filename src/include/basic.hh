#ifndef BASIC_CONFIG
#define BASIC_CONFIG

#define debug_switch 0
#define debug(statement) \
  if (debug_switch) statement;

#define LOCAL_PORT 30000
#define TCP_SEQ_NUM 0x1234
#define TCP_WIN_SIZE 0x4000
#define CUSTOM_IP_ID 0x1000
#define CUSTOM_IP_TTL 64
#define MAX_PACKET_LENTH 8192
#define RETRY_TIMES 2
#define RECV_TIMEOUT_SEC 1

enum scan_type {
  UDP_SCAN = 1,
  TCP_SYN_SCAN = 1 << 1,
  TCP_CNN_SCAN = 1 << 2,
  TCP_ACK_SCAN = 1 << 3,
  PING_SWEEP = 1 << 4,
};

enum error_type {
  TARGET_PRASE_FAILED = -1,
  PORT_OUT_OF_RANGE = -2,
  SOCKET_CREATE_ERROR = -3,
  SOCKET_SEND_ERROR = -4,
  SOCKET_RECV_ERROR = -5,
  UNKNOWN_RESULT = -6,
  SOCK_BIND_ERROR = -7,
  SOCK_CONN_ERROR = -8
};

enum syn_res {
  RST = 1,
  ACK = 2,
};

#endif
