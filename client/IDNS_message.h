#include <iostream>
#include <memory>
#include "DNS_parts.h"

class IDNS_message
{
public:
    virtual void add_query(dns::query*q)=0;
    virtual char*get_packet_data(int &size)=0;
    virtual ~IDNS_message(){};

};