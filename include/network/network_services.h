#pragma once

#include <winsock2.h>
#include <Windows.h>

namespace network
{
	namespace services
	{
		int SendMessage(SOCKET cur_socket, char* message, int message_size);
		int ReceiveMessage(SOCKET cur_socket, char* buffer, int bufsize);
	};
}

