#include "server.h"

Server*Server::m_ptr=nullptr;

Server::Server():m_port(APPLICATION_PORT)
{
    m_sock=create_sock();
    initialize_addr(m_addr,APPLICATION_IP,m_port);
    bind_sock(m_sock,m_addr);

    m_sock=create_sock();
    initialize_addr(m_dns_addr,m_dns_servers[0].c_str(),53);

    get_dns_servers();
}


int Server::create_sock()
{
   int sock=socket(AF_INET,SOCK_DGRAM,0);

    if(sock<0)
    {
        perror("socket creation failed");
        exit(1);
    }

    return sock;
}

void Server::initialize_addr(struct sockaddr_in& addr,const char *ip_address,const int& port)
{
    memset(&addr, 0, sizeof(addr));       
    addr.sin_family = AF_INET;                //Ipv4
    addr.sin_addr.s_addr = inet_addr(ip_address);     //test ->binar
    addr.sin_port = htons(port);
}

void Server::bind_sock(int &sock, const sockaddr_in &addr)
{
    if(bind(sock,(const struct sockaddr*)&addr,sizeof(addr))<0)
    {
        perror("Error on bind");
        close(m_sock);
        exit(1);
   }
}

void Server::get_dns_servers()
{
    std::ifstream p_file("/etc/resolv.conf");

    if(!p_file.is_open())
    {
        std::cerr<<"Eroare la deschiderea fisierului\n";
        exit(1);
    }

    std::string line;

    while(getline(p_file,line))
    {
        if(line[0]!='#')
        {
            add_dns_ip(line);
        }
    }
}

void Server::add_dns_ip(const std::string &line)
{
    std::istringstream iss(line);
    std::string wrd1,wrd2;
    iss>>wrd1>>wrd2;
    m_dns_servers.push_back(wrd2);
}

Server &Server::get_instance()
{
    if(m_ptr==nullptr)
        m_ptr=new Server();

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
  char buffer[1024];
  struct sockaddr_in client_addr;
  socklen_t addr_len = sizeof(client_addr);
    
    while (1)
    {
        int recv_bytes = recvfrom(m_sock, buffer, 1023, 0, (struct sockaddr*)&client_addr, &addr_len);
        
        if (recv_bytes > 0)
        {
            buffer[recv_bytes] = '\0';

            std::cout << "Client msg: " << buffer << std::endl;

            

            //sendto(m_sock, buffer, strlen(buffer), 0, (struct sockaddr*)&client_addr, addr_len);
        }

    }

}



