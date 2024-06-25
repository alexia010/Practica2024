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
    memset(&m_addr,0,sizeof(m_addr));

    m_addr.sin_family=AF_INET;             //IPv4
    m_addr.sin_port=htons(PORT);           //port
    
    if(inet_pton(AF_INET,ip_dest,&m_addr.sin_addr)<=0)      // 0- ip invalid -1 eroare 1 succes
    {
        std::cerr<<"Error connection";
        exit(1);
    }
    m_addr.sin_addr.s_addr=INADDR_ANY;

}

void Client::send_request()
{
    std::cout<<"Enter hostname to lookup: ";

    std::string hostname;
    std::cin>>hostname;
    std::cout<<"Record type:\n";
    dns::print_record_type();

    std::string type;
    std::cout<<"Enter record type:\n";
    std::cin>>type;

    dns::dns_record_type r_type=dns::check_type(type);

    CDNS_request request;
    request.set_header(1,0,0,0,0,0,0,0,0,0,1,0,0,0);

    dns::query*q=new dns::query(hostname,dns::enum_to_int(r_type),1); //1 internet
    request.add_query(q);

    std::cout<<"Sending packet:\n";
   
    int size=0;
    char*ptr=nullptr;

    request.get_packet_data(ptr,size);
    int bytes_sent=sendto(m_sock_fd,ptr,size,0,(struct sockaddr*)&m_addr,sizeof(m_addr));

    if( bytes_sent< 0)
    {
        perror("sendto failed");
        exit(1);
    }

    ptr=m_dns_response;
    socklen_t addr_len=sizeof(m_addr);
    int recv_bytes=recvfrom(m_sock_fd,ptr,65536,0,(struct  sockaddr*)&m_addr,(socklen_t*)&addr_len);
    
    if(recv_bytes< 0)
    {
        perror("recvfrom failed");
    }

    std::cout<<"Mesaj server: "<<m_dns_response;


}

void Client::send_message(const char *msg)
{
    int sent_bytes=sendto(m_sock_fd, msg, strlen(msg),
    MSG_CONFIRM, (const struct sockaddr *) &m_addr,sizeof(m_addr));

    if(sent_bytes<=0)
    {
        perror("sendto failed");
    }
}
