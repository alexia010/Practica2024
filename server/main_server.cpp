#include <iostream>
#include "server.h"


int main()
{
    Server& server=Server::get_instance();
    server.run();
    return 0;
}
