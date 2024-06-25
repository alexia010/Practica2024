#include "server.h"


Server*Server::m_ptr=nullptr;

Server::Server():m_port(APPLICATION_PORT)
{

    m_sock=create_sock();
    initialize_addr(m_addr,APPLICATION_IP,m_port);
    bind_sock(m_sock,m_addr);

    get_dns_servers();
    m_dns_sock=create_sock();

    std::string google="Google Public DNS";
    auto it = m_dns_servers.find(google);
    initialize_addr(m_dns_addr,it->second.c_str(),53);

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
     std::ifstream p_file("dns_servers.txt");
    if(!p_file.is_open())
    {
        std::cerr<<"Eroare la deschiderea fisierului\n";
        exit(1);
    }
    std::string line;
    while(getline(p_file,line))
    {
        add_dns_server_in_map(line);
    }

    p_file.close();
}

void Server::add_dns_server_in_map(std::string line)
{
    std::istringstream iss(line);
    std::string ip,name;

    if(iss>>ip&&std::getline(iss,name))
    {
        name=name.substr(1);
        m_dns_servers[name]=ip;
    }

}

void Server::query_dns_server(char *&ptr, int &size,const sockaddr_in & client_addr,const socklen_t & addr_len)
{
    int sent_bytes=sendto(m_dns_sock,ptr,size,0,(struct sockaddr*)&m_dns_addr,sizeof(m_dns_addr));
    if(sent_bytes < 0)
    {
        perror("sendto failed");
    }
    
    std::cout<<"Query sent to dns server...\n";
    
    std::cout<<"Receiving answear...\n";

    socklen_t len=sizeof(m_addr);
    int recv_bytes=recvfrom (m_dns_sock,(char*)m_dns_response, 65536 , 0 , (struct sockaddr*)&m_dns_addr , &len );
    if(recv_bytes< 0)
    {
        perror("recvfrom failed");
    }

    ptr=m_dns_response;

    sent_bytes=sendto(m_sock,ptr,recv_bytes,0,(struct sockaddr*)&client_addr,sizeof(client_addr));

    if(sent_bytes < 0)
    {
        perror("sendto failed");
    }
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
  char buffer[65536];
  struct sockaddr_in client_addr;
  socklen_t addr_len = sizeof(client_addr);

    
    while (1)
    {
        int recv_bytes = recvfrom(m_sock, (char*)buffer, 65535, MSG_WAITALL, (struct sockaddr*)&client_addr, &addr_len);
        
        if (recv_bytes > 0)
        {
            buffer[recv_bytes] = '\0';

            std::cout << "Client msg: " << buffer << std::endl;

            char*ptr=nullptr;
            ptr=buffer;
            int size=recv_bytes;

            query_dns_server(ptr,size,client_addr,addr_len);


            //sendto(m_sock, buffer, strlen(buffer), 0, (struct sockaddr*)&client_addr, addr_len);
        }

    }

}



