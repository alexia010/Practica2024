#include <iostream>
#include "client.h"


int main()
{
    Client client;
    client.connect("127.0.0.1");
    client.send_request();
    
    for(;;)
    {
      //  client.send_message((const char*)"Mesaj");
      // std::cout<<"Mesaj\n";
        
    }
	
    return 0;
}

