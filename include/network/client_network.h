/*
 *
 * File: client_network.h
 *
 * Author: Yaroslav Kishchuk
 * Contact: kshchuk@gmail.com
 *
 */


#pragma once

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
    // Size of our buffer
    const int kDefaultBufflen = 512;

    // Stores network info
    class ClientNetwork
    {
    public:

        // IP/PORT to connect sockets through 
        char* ip_;
        char* port_;

        // For error checking function calls in Winsock library
        int result_;

        // Socket for client to connect to server
        SOCKET connected_socket_;

        // ctor/dtor
        ClientNetwork(char* ip, char* port);
        ~ClientNetwork(void);

        // Returns error number if fails
        int ReceivePackets(char* recvbuf);
    };

}