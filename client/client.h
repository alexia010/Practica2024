#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 1234

class Client
{
private:

    short m_port;
    int m_sock_fd;
    
    struct sockaddr_in m_addr;


public:
    Client();
    ~Client();

    void connect(const char*ip_dest);
    void send_message(const char*msg);
};