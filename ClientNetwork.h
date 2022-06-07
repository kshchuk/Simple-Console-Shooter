#pragma once

// Networking libraries
#include <winsock2.h>
#include <Windows.h>
#include "NetworkServices.h"
#include "NetworkData.h"
#include <ws2tcpip.h>
#include <stdio.h> 

// size of our buffer
constexpr int DEFAULT_BUFLEN = 512;

// ip/port to connect sockets through 
constexpr char SERVER_IP[] = "10.11.27.20";
constexpr char DEFAULT_PORT[] = "6881";

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


class ClientNetwork
{
public:

    // for error checking function calls in Winsock library
    int iResult;

    // socket for client to connect to server
    SOCKET ConnectSocket;

    // ctor/dtor
    ClientNetwork(void);
    ~ClientNetwork(void);
};

