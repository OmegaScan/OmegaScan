#ifndef TCP_SYN
#define TCP_SYN

#include <arpa/inet.h>
#include <strings.h>
#include <sys/socket.h>

#include <iostream>
#include <string>

#include "basic.hh"

/**
 * @brief           Sacn port by SYN SCAN
 * @param host      Target host ipv4
 * @param port      Target port
 *
 * @return
 *      return < 0: Something unexpected happens, more details can be found
 *                  at basic.hh -> error_type
 *      return > 0: Succeed Scan, more details can be found at basic.hh ->
 *                  syn_res
 **/
int tcp_syn(std::string host, unsigned short port,
            unsigned short local_port = LOCAL_PORT);
// int tcp_syn(std::string host, unsigned short port);

#endif
