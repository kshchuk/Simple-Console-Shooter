/*
 *
 * File: network_services.h
 *
 * Author: Yaroslav Kishchuk
 * Contact: kshchuk@gmail.com
 *
 */


#pragma once

#include <winsock2.h>
#include <Windows.h>


namespace network
{
	namespace services
	{
		static int SendMessage(SOCKET cur_socket, char* message, int message_size) {
			return send(cur_socket, message, message_size, 0);
		}

		static int ReceiveMessage(SOCKET cur_socket, char* buffer, int bufsize) {
			return recv(cur_socket, buffer, bufsize, 0);
		}
	};
}

