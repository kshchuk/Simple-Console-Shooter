#include "../include/network/network_services.h"

int network::services::SendMessage(SOCKET cur_socket, char* message, int message_size)
{
    return send(cur_socket, message, message_size, 0);
}

int network::services::ReceiveMessage(SOCKET cur_socket, char* buffer, int buf_size)
{
    return recv(cur_socket, buffer, buf_size, 0);
}