#pragma once
#include <iostream>
#include <vector>
#include "IDNS_message.h"


class CDNS_request:public IDNS_message
{
protected:

    dns::DNS_header *header;
    std::vector<dns::query*> queries;     

public:
    CDNS_request():header(nullptr){};
    virtual ~CDNS_request();

    void add_query(dns::query*q)override{queries.push_back(q);};
    virtual void get_packet_data(char*&ptr,int &size)override;
    void set_header( bool rd, bool tc, bool aa, unsigned char opcode, bool qr, unsigned char rcode,
                bool cd, bool ad, bool z, bool ra, unsigned short q_count, unsigned short ans_count,
                unsigned short auth_count, unsigned short add_count);
    
};