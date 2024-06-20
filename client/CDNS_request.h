#pragma once
#include <iostream>
#include <vector>
#include "IDNS_message.h"

class CDNS_request:public IDNS_message
{
protected:
    dns::DNS_header *header;
    std::vector<std::unique_ptr<dns::question>> *questions;
public:
    CDNS_request():header(nullptr){};
    virtual ~CDNS_request();

    void add_question(std::unique_ptr<dns::question>&q)override{questions->push_back(q);};

};