#include "server.h"

Server*Server::m_ptr=nullptr;

Server::Server(const char* ip_address):m_port(PORT)
{
    m_listen_sock=create_sock();
    set_sock_non_blocking(m_listen_sock);
    bind_sock(m_listen_sock,m_addr);
    //listen(m_listen_sock);
}


int Server::create_sock()
{
   int sock=socket(AF_INET,SOCK_DGRAM,0);

    if(sock<0)
    {
        std::cerr<<"Failed creating socket";
        exit(EXIT_FAILURE);
    }

    return sock;
}

void Server::initialize_addr(const char *ip_address)
{
    memset(&m_addr, 0, sizeof(m_addr));
    m_addr.sin_family = AF_INET;
    m_addr.sin_addr.s_addr = inet_addr(ip_address); 
    m_addr.sin_port = htons(m_port);
}


void Server::set_sock_non_blocking(int &sock)
{
    int flags=fcntl(sock,F_GETFL,0);

    if(flags== -1)
    {
        std::cerr<<"Failed getting listen sock flags";
        return;
    }

    flags|=O_NONBLOCK;

    if( fcntl(sock,F_SETFL,flags)<0)
    {
        std::cerr<<"Failed adding non block flag";
        return;
    }
}

void Server::bind_sock(int sock, const sockaddr_in &m_addr)
{
     if (bind(sock, (const struct sockaddr*)&m_addr, sizeof(m_addr)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        close(sock);
        exit(EXIT_FAILURE);
    }
}

void Server::listen(int &sock)
{
    if(::listen(sock,SOMAXCONN)<0)
    {
        std::cerr<<"Listen failed.\n";
        close(sock);
        exit(1);
    }
}

void Server::init_master(fd_set &master)
{
    FD_ZERO(&master);
    FD_SET(m_listen_sock,&master);
}

Server &Server::get_instance()
{
    if(m_ptr==nullptr)
        m_ptr=new Server((const char*)"127.0.0.1");

    return *m_ptr;
}

void Server::destroy_instance()
{
    if(m_ptr!=nullptr)
    {
        delete m_ptr;
        m_ptr=nullptr;
    }
}

void Server::run()
{
    fd_set master;
    init_master(master);

    while(1)
    {   
        handle_incoming_connections(master);
        handle_client_data(master);
    }
}

void Server::handle_incoming_connections(fd_set &master)
{
    int new_client_sock=accept(m_listen_sock,nullptr,nullptr);

    if(new_client_sock!=-1)
    {
        FD_SET(new_client_sock,&master);
        m_client_socks.push_back(new_client_sock);

        std::cout<<"Client connected!\n";

    }
}

void Server::handle_client_data(const fd_set &master)
{
    for(int sock:m_client_socks)
    {
        if(FD_ISSET(sock,&master))
        {
            char buffer[1024];

            struct sockaddr_in client_addr;
            socklen_t addr_len=sizeof(client_addr);
    
            int recv_bytes=::recvfrom(sock,buffer,1023,0,(struct sockaddr*)&client_addr,&addr_len);

            if(recv_bytes<=0)
            {
                continue;
            }
            else
            {
                buffer[recv_bytes]='\0';
                std::cout<<buffer;
            }
        }
    }
}
