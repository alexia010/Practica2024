#pragma once
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "CDNS_request.h"

#define PORT 1234

class Client
{
private:

    short m_port;
    int m_sock_fd;
    struct sockaddr_in m_addr;

    char m_dns_response[65536];

  
public:
    Client();
    ~Client();

    void connect(const char*ip_dest);
    void send_request();

    void send_message(const char*msg);
};