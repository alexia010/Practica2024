#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>      //flag uri sokceturi

#include <vector>

#define PORT 53

class Server
{
private:

    static Server* m_ptr;
    Server(const char* ip_address);
    Server(const Server&obj){};
    ~Server(){};

    short m_port;
    int m_listen_sock;
    struct sockaddr_in m_addr;

    std::vector<int> m_client_socks;

    int create_sock();
    void initialize_addr(const char*ip_address);
    void set_sock_non_blocking(int &sock);
    void bind_sock(int sock,const sockaddr_in &m_addr);
    void listen(int &sock);

    void init_master(fd_set&master);
    void handle_incoming_connections(fd_set& master);
    void handle_client_data(const fd_set&master);

public:
    static Server&get_instance();
    static void destroy_instance();
    void run();
    
};