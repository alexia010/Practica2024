#include <iostream>
#include <memory>
#include "DNS_parts.h"

class IDNS_message
{
public:
    virtual void add_question(std::unique_ptr<dns::question>&qst)=0;
    virtual ~IDNS_message(){};

};