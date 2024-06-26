#include "CDNS_request.h"
#include <cstring>

CDNS_request::~CDNS_request()
{
    if(header!=nullptr)
    {
        delete header;
        header=nullptr;
    }
}

void CDNS_request::get_packet_data(char*&ptr,int &size)
{
    static char buffer[65536];  //max dns packet size

    memset(buffer, 0, sizeof(buffer));
    ptr=buffer;

    size=0;

    memcpy(ptr, header, sizeof(dns::DNS_header));
    ptr += sizeof(dns::DNS_header);
    size+=sizeof(dns::DNS_header);

     for (const auto& q : queries)
    {
        q->get_data(ptr,size);
    }

    ptr=buffer;
}

void CDNS_request::set_header(bool rd, bool tc, bool aa, unsigned char opcode, bool qr, unsigned char rcode, bool cd, bool ad, bool z, bool ra, unsigned short q_count, unsigned short ans_count, unsigned short auth_count, unsigned short add_count)
{
    if(header==nullptr)
    {
        header=new dns::DNS_header;
    }

    header->set_all(rd,tc,aa,opcode,qr,rcode,cd,ad,z,ra,q_count,ans_count,auth_count,add_count);

}
