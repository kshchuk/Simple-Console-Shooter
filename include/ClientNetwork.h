#pragma once

// Networking libraries
#include <stdio.h> 
#include <winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>

#include "NetworkServices.h"
#include "NetworkData.h"

// size of our buffer
constexpr int DEFAULT_BUFLEN = 512;

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


class ClientNetwork
{
public:

    // ip/port to connect sockets through 
    char* SERVER_IP;
    const char* DEFAULT_PORT = new char[] {"6881"};

    // for error checking function calls in Winsock library
    int iResult;

    // socket for client to connect to server
    SOCKET ConnectSocket;

    // ctor/dtor
    ClientNetwork(char* SERVER_IP);
    ~ClientNetwork(void);

    int receivePackets(char*);
};

