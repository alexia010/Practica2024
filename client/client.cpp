#include "client.h"
#include "CDNS_response.h"
#include <fstream>

#include <ctime>
#include <iomanip>
#include <chrono>

void Client::print_current_time()
{
    time_t now=time(nullptr);
    tm*tm_now=localtime(&now);

    std::cout<<std::put_time(tm_now,"%a %b %d %H:%M:%S %Z %Y")<<"\n";
}

void Client::print_stats(int t_time, int rsp_size)
{
    std::cout<<"****STATS:\n";
    std::cout<<"Total query time: "<<t_time<<"\n";
    std::cout<<"Date: ";
    print_current_time();
    std::cout<<"DNS response size: "<<rsp_size<<"bytes\n";

}

Client::Client() : m_port(PORT), m_sock_fd(-1)
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

    auto start_time=std::chrono::high_resolution_clock::now();

    std::cout<<"\n\n**** QUESTION SECTION:\n"<<hostname<<"\t\t"<<"IN\t\t"<<type<<"\n\n";
    CDNS_request request;
    request.set_header(1,0,0,0,0,0,0,0,0,0,1,0,0,0);

    int qname_size=hostname.size();
    dns::query*q=new dns::query(hostname,dns::enum_to_int(r_type),1); //1 internet
    request.add_query(q);

    std::cout<<"\n";
   
    int size=0;
    unsigned char*ptr=nullptr;

    request.get_packet_data(ptr,size);
    int bytes_sent=sendto(m_sock_fd,ptr,size,0,(struct sockaddr*)&m_addr,sizeof(m_addr));

    if( bytes_sent< 0)
    {
        perror("sendto failed");
        exit(1);
    }

    ptr=m_dns_response;
    socklen_t addr_len=sizeof(m_addr);
    int recv_bytes_dns=recvfrom(m_sock_fd,ptr,65536,0,(struct  sockaddr*)&m_addr,(socklen_t*)&addr_len);
    
    if(recv_bytes_dns< 0)
    {
        perror("recvfrom failed");
    }


    ptr=m_dns_response;
    size=0;

    unsigned char*rsp=m_dns_response;

    CDNS_response response;
    response.set_name_size(qname_size);
    response.populate_response(ptr,rsp);

    auto end_time=std::chrono::high_resolution_clock::now();
    auto total_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    response.print_result();
    print_stats(total_time.count(),recv_bytes_dns);

}

