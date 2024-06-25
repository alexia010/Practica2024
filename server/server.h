#pragma once
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <sstream>
#include <vector>
#include <fstream>
#include <map>

#define APPLICATION_PORT 1234
#define APPLICATION_IP "127.0.0.1"  //loopback adress


class Server
{
private:

    static Server* m_ptr;
    Server();
    Server(const Server&obj){};
    ~Server(){};

    short m_port;
    int m_sock;
    struct sockaddr_in m_addr;

    short m_dns_port;
    int m_dns_sock;
    struct sockaddr_in m_dns_addr;

    std::map<std::string,std::string>m_dns_servers;
    
    char m_dns_response[65536];

    int create_sock();
    void initialize_addr(struct sockaddr_in& addr,const char *ip_address,const int& port);
    void bind_sock(int &sock, const sockaddr_in &addr);

   void get_dns_servers();
   void add_dns_server_in_map(std::string line);
    void query_dns_server(char*&ptr,int &size,const sockaddr_in & client_addr,const socklen_t & addr_len);

public:
    static Server&get_instance();
    static void destroy_instance();
    void run();
    
};