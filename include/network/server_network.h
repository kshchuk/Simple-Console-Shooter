#pragma once

#include <winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>

#include <map>
#include <string>

#include "network_services.h"
#include "network_data.h"


#pragma comment (lib, "Ws2_32.lib")

namespace network
{
    const int kDefaultBuflen = 512;

    class ServerNetwork
    {
    public:

        // Socket to listen for new connections
        SOCKET listen_socket_;

        // Socket to give to the clients
        SOCKET client_socket_;

        // For error checking return values
        int connect_result_;

        // Table to keep track of each client's socket
        std::map<unsigned int, SOCKET> sessions_;

        ServerNetwork(std::string port);
        ~ServerNetwork(void);

        // Accept new connections
        bool AcceptNewClient(unsigned int& id);

        // Receive incoming data
        int ReceiveData(unsigned int client_id, char* recvbuf);

        // Send data to all clients
        void SendToAll(char* packets, int totalSize);

        char* GetMyPublicIP();
    };
}
