#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#define MAGIC "WanderingSinger"
#define MAGIC_LEN 15
#define MTU 1500

struct icmp_echo {

    // 报文头部
    uint8_t type;
    uint8_t code;
    uint16_t checksum;

    uint16_t ident;
    uint16_t seq;

    // 数据部分
    double sending_ts;
    char magic[MAGIC_LEN];
};

class pinger {
public:
    enum icmp_rec_state {
        ICMP_CORRECT_RECV = 0,
        ICMP_UNEXCEPT_TYPE_RECV = 1,
        ICMP_UNEXCEPT_IDENT_RECV = 2,
        ICMP_NO_RECV = 3
    };

private:
    int sockfd;
    struct sockaddr_in addr;
    struct icmp_echo icmp_request;
    struct icmp_echo icmp_reply;

    uint16_t ident;
    uint16_t seq;

public:
    pinger(const char *ip);
    ~pinger();

    /**
     * @brief 在指定时间内一直ping，直到ping成功
     *
     * @param send_gap_time 两次ping之间的时间间隔
     * @param timeout 超时时间（即函数函数最长运行时间）
     * @return 0表示ping成功，-1表示失败
     */
    int ping_for_success(double send_gap_time, double timeout);

    /**
     * @brief ping一次
     *
     * @param timeout 超时时间
     * @return 0表示ping成功，-1表示失败
     */
    int ping(double timeout);

    /**
     * @brief 获取最后一次ping中ICMP报文的序列号
     *
     */
    int get_seq();

    /**
     * @brief ping的目标地址
     *
     */
    char* get_addr();

    /**
     * @brief 获取最后一次ping中ICMP报文的发送时间
     *
     */
    double get_sending_ts();

    static uint16_t calculate_checksum(unsigned char* buffer, int bytes);

private:

    void init_address(const char *ip);
    void init_socket();
    void init_socket_timeout(time_t sec, time_t usec);
    void init_icmp_request();

    int send_icmp_request();
    int recv_icmp_reply();

};
