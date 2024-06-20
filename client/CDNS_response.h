#pragma once
#include "CDNS_request.h"

class CDNS_response:public CDNS_request
{
private:
    std::vector<std::unique_ptr<dns::resource_record>>m_answers;
    std::vector<std::unique_ptr<dns::resource_record>>m_authorities;
    std::vector<std::unique_ptr<dns::resource_record>>m_additionals;
public:
    CDNS_response();
    ~CDNS_response(){};
};

