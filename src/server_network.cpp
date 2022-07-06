
#include <stdlib.h>
#include <stdio.h>

#include <string>
#include <iostream>

#include "../include/network/server_network.h"

#include <windows.h> 
#include <wininet.h> 



namespace network
{
    ServerNetwork::ServerNetwork(std::string port)
    {

        // Create WSADATA object
        WSADATA wsa_data;

        // Our sockets for the server
        listen_socket_ = INVALID_SOCKET;
        client_socket_ = INVALID_SOCKET;

        // Address info for the server to listen to
        struct addrinfo* result = NULL;
        struct addrinfo hints;

        // Initialize Winsock
        connect_result_ = WSAStartup(MAKEWORD(2, 2), &wsa_data);
        if (connect_result_ != 0) {
            printf("WSAStartup failed with error: %d\n", connect_result_);
            system("PAUSE");
            exit(1);
        }

        // Set address information
        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;    // TCP connection!!!
        hints.ai_flags = AI_PASSIVE;

        connect_result_ = getaddrinfo(NULL, port.c_str(), &hints, &result);

        if (connect_result_ != 0) {
            printf("getaddrinfo failed with error: %d\n", connect_result_);
            system("PAUSE");
            WSACleanup();
            exit(1);
        }

        // Create a SOCKET for connecting to server
        listen_socket_ = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

        if (listen_socket_ == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            freeaddrinfo(result);
            system("PAUSE");
            WSACleanup();
            exit(1);
        }

        // Set the mode of the socket to be nonblocking
        u_long iMode = 1;
        connect_result_ = ioctlsocket(listen_socket_, FIONBIO, &iMode);

        if (connect_result_ == SOCKET_ERROR) {
            printf("ioctlsocket failed with error: %d\n", WSAGetLastError());
            system("PAUSE");
            closesocket(listen_socket_);
            WSACleanup();
            exit(1);
        }

        // Setup the TCP listening socket
        connect_result_ = bind(listen_socket_, result->ai_addr, (int)result->ai_addrlen);

        if (connect_result_ == SOCKET_ERROR) {
            printf("bind failed with error: %d\n", WSAGetLastError());
            system("PAUSE");
            freeaddrinfo(result);
            closesocket(listen_socket_);
            WSACleanup();
            exit(1);
        }

        // No longer need address information
        freeaddrinfo(result);

        // Start listening for new clients attempting to connect
        connect_result_ = listen(listen_socket_, SOMAXCONN);

        if (connect_result_ == SOCKET_ERROR) {
            printf("listen failed with error: %d\n", WSAGetLastError());
            system("PAUSE");
            closesocket(listen_socket_);
            WSACleanup();
            exit(1);
        }

        std::cout << "\n      Server IP:  " << std::string(GetMyPublicIP()) << "      PORT:   " << port << "\n\n";
    }


    // Accept new connections
    bool ServerNetwork::AcceptNewClient(unsigned int& id)
    {
        // If client waiting, accept the connection and save the socket
        client_socket_ = accept(listen_socket_, NULL, NULL);

        if (client_socket_ != INVALID_SOCKET)
        {
            // Disable nagle on the client's socket
            char value = 1;
            setsockopt(client_socket_, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));

            // Insert new client into session id table
            sessions_.insert(std::pair<unsigned int, SOCKET>(id, client_socket_));

            return true;
        }

        return false;
    }

    char* ServerNetwork::GetMyPublicIP() {

        FILE* curl;
        if ((curl = _popen("curl https://icanhazip.com/", "r")) == NULL) {
            char* temp = new char[] { "None. Failed to run curl" };
            return temp;
        }

        char* ip = new char[99];
        if (fgets(ip, 99, curl) == NULL) {
            char* temp = new char[] { "None. (do you have internet connection?)" };
            return temp;
        }
        system("cls");
        return ip;
    }

    // Receive incoming data
    int ServerNetwork::ReceiveData(unsigned int client_id, char* recvbuf)
    {
        if (sessions_.find(client_id) != sessions_.end())
        {
            SOCKET current_socket = sessions_[client_id];
            connect_result_ = network::services::ReceiveMessage(current_socket, recvbuf, kMaxPacketSize);

            if (connect_result_ == 0)
            {
                printf("Connection closed\n");
                closesocket(current_socket);
            }
            return connect_result_;
        }
        return 0;
    }

    // Send data to all clients
    void ServerNetwork::SendToAll(char* packets, int total_size)
    {
        SOCKET current_socket;
        std::map<unsigned int, SOCKET>::iterator iter;
        int send_result;

        for (iter = sessions_.begin(); iter != sessions_.end(); iter++)
        {
            current_socket = iter->second;
            send_result = network::services::SendMessage(current_socket, packets, total_size);

            if (send_result == SOCKET_ERROR)
            {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(current_socket);
            }
        }
    }
}