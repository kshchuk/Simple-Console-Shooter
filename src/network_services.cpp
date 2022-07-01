#include "../include/network/network_services.h"

namespace network_services
{
    int sendMessage(SOCKET curSocket, char* message, int messageSize)
    {
        return send(curSocket, message, messageSize, 0);
    }

    int receiveMessage(SOCKET curSocket, char* buffer, int bufSize)
    {
        return recv(curSocket, buffer, bufSize, 0);
    }
}