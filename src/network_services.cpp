#include "../include/network/network_services.h"

int network_services::sendMessage(SOCKET curSocket, char* message, int messageSize)
{
    return send(curSocket, message, messageSize, 0);
}

int network_services::receiveMessage(SOCKET curSocket, char* buffer, int bufSize)
{
    return recv(curSocket, buffer, bufSize, 0);
}