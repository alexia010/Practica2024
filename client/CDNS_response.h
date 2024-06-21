#pragma once
#include "CDNS_request.h"

class CDNS_response:public CDNS_request
{
private:
    std::vector<dns::resource_record*>m_answers;
    std::vector<dns::resource_record*>m_authorities;
    std::vector<dns::resource_record*>m_additionals;
public:
    CDNS_response();
    ~CDNS_response(){};
};

