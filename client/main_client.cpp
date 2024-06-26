#include <iostream>
#include "client.h"


int main()
{
    Client client;
    client.connect("127.0.0.1");
    client.send_request();

    return 0;
}

