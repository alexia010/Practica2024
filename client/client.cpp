#include "client.h"

Client::Client():m_port(PORT),m_sock_fd(-1)
{
    m_sock_fd=socket(AF_INET,SOCK_DGRAM,0);

    if(m_sock_fd<0)
    {
        std::cerr<<"Failed creating socket";
        exit(EXIT_FAILURE);
    }

}

Client::~Client()
{
    close(m_sock_fd);
}

void Client::connect(const char *ip_dest)
{
    std::string port_str=std::to_string(PORT);

    memset(&m_addr,0,sizeof(m_addr));

    m_addr.sin_family=AF_INET;             //IPv4
    m_addr.sin_port=htons(PORT);           //port
    
    if(inet_pton(AF_INET,ip_dest,&m_addr.sin_addr)<0)
    {
        std::cerr<<"Error connection";
        exit(1);
    }

}

void Client::send_message(const char *msg)
{
    sendto(m_sock_fd, (const char *)"hello", strlen("hello"),MSG_CONFIRM, (const struct sockaddr *) &m_addr,sizeof(m_addr));
}
