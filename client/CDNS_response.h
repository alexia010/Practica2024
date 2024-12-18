#pragma once
#include "CDNS_request.h"

class CDNS_response:public CDNS_request
{
private:
    std::vector<dns::resource_record*>m_answers;
    std::vector<dns::resource_record*>m_authorities;
    std::vector<dns::resource_record*>m_additionals;

    void populate_header(unsigned char*&ptr);
    void populate_queries(unsigned char*&ptr);
    void populate_answers(unsigned char*&ptr,unsigned char*&response);
    void populate_authorities(unsigned char *&ptr,unsigned char*&response);
    void populate_additionals(unsigned char *&ptr,unsigned char*&response);

    void print_answers();
    void print_answear_by_type(dns::resource_record*ans);
    void print_ipv4(std::string rdata);
    void print_authorities();
    void print_additionals();

    int m_qname_size;
public:

    void populate_response(unsigned char*&ptr,unsigned char*&response);
    void set_name_size(int size){m_qname_size=size;};
    void print_result();

    CDNS_response();
    ~CDNS_response();
};

