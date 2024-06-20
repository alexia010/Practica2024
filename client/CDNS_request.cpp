#include "CDNS_request.h"

CDNS_request::~CDNS_request()
{
    if(header!=nullptr)
    {
        delete header;
        header=nullptr;
    }
}
