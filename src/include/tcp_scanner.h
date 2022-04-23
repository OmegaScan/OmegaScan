#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <vector>
#include <algorithm>

class tcpScanner {
private:

    std::vector<int> ports;
    u_long addr;

    /**
     * @brief 传给select函数的fd集
     * 
     */
    fd_set fdset;

    /**
     * @brief 记录创建的socket对应的fd
     * 
     */
    std::vector<int> sockfds;
    
    /**
     * @brief select函数的超时时间
     * 
     */
    timeval timeout;

    /**
     * @brief 扫描到开放的端口后调用的回调函数，
     * 传入的参数依次为ip，port，fd
     * 
     */
    void (*successCallback)(u_long, int, int);

    /**
     * @brief 根据ip和ports创建若干个sockets
     * 
     * @return 创建成功的socket数
     */
    int createSockets();

    /**
     * @brief 把所有的socket连接
     * 
     */
    void connectSockets();

    /**
     * @brief 只要有socket连接成功，或超时，函数就会返回
     * 
     * @return 连接成功的socket数
     */
    int waitConnectionSuccess();

    /**
     * @brief 关闭所有的socket连接
     * 
     */
    void closeAllFd();

    /**
     * @brief 获取连接成功的socket对应的下标
     * 
     */
    std::vector<int> getSuccessSocksIndex();

public:
    /**
     * @brief 设置超时时间
     * 
     */
    void setTimeout(timeval timeout);

    /**
     * @brief 扫描到一个开放的端口时，调用此回调函数
     * 
     * @param f 回调函数的3个参数依次为ip,port,fd
     */
    void setSuccessCallback(void (*f)(u_long, int, int));

    /**
     * @brief 执行扫描
     * 
     * @param addr ip地址（32位二进制值，host字节序）
     * @param ports 端口号集
     */
    void scan(u_long addr, const std::vector<int>& ports);

    tcpScanner();
    ~tcpScanner();
};
