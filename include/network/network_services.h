#pragma once

#include <winsock2.h>
#include <Windows.h>

namespace network_services
{
	int sendMessage(SOCKET curSocket, char* message, int messageSize);
	int receiveMessage(SOCKET curSocket, char* buffer, int bufSize);
};
