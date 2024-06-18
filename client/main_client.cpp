#include <iostream>
#include "client.h"


int main()
{
    Client client;
    client.connect("127.0.0.1");
    
    for(;;)
    {
        client.send_message((const char*)"Mesaj");
        
    }
	
    return 0;
}

