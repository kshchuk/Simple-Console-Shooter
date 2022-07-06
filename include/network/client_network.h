#pragma once

// Networking libraries
#include <winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
#include <stdio.h> 
#include <string>

#include "network_services.h"
#include "network_data.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


namespace network
{
    // size of our buffer
    const int kDefaultBufflen = 512;

class ClientNetwork
{
public:

    // ip/port to connect sockets through 

    // for error checking function calls in Winsock library

    // socket for client to connect to server

    // ctor/dtor
    ~ClientNetwork(void);

};

}