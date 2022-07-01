#include "../include/network/client_network.h"

namespace network
{

    ClientNetwork::ClientNetwork(std::string ip, std::string port)
    {

        ip_ = ip;
        port_ = port;

        // Сreate WSADATA object
        WSADATA wsa_data;

        // Socket
        connected_socket_ = INVALID_SOCKET;

        // Holds address info for socket to connect to
        struct addrinfo* connect_info = nullptr,
            * ptr = nullptr,
            hints;

        // Initialize Winsock
        result_ = WSAStartup(MAKEWORD(2, 2), &wsa_data);

        if (result_ != 0) {
            printf("WSAStartup failed with error: %d\n", result_);
            system("PAUSE");
            return;
        }

        // Set address info
        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;  //TCP connection!!!

        // Resolve server address and port
        result_ = getaddrinfo(ip_.c_str(), port_.c_str(), &hints, &connect_info);

        if (result_ != 0)
        {
            printf("getaddrinfo failed with error: %d\n", result_);
            system("PAUSE");
            WSACleanup();
            return;
        }

        // Attempt to connect to an address until one succeeds
        for (ptr = connect_info; ptr != NULL; ptr = ptr->ai_next) {

            // Create a SOCKET for connecting to server
            connected_socket_ = socket(ptr->ai_family, ptr->ai_socktype,
                ptr->ai_protocol);

            if (connected_socket_ == INVALID_SOCKET) {
                printf("socket failed with error: %ld\n", WSAGetLastError());
                system("PAUSE");
                WSACleanup();
                return;
            }

            // Connect to server.
            result_ = connect(connected_socket_, ptr->ai_addr, static_cast<int> (ptr->ai_addrlen));

            if (result_ == SOCKET_ERROR)
            {
                closesocket(connected_socket_);
                connected_socket_ = INVALID_SOCKET;
                printf("The server is down... did not connect\n");
                system("PAUSE");
            }
        }



        // no longer need address info for server
        freeaddrinfo(connect_info);



        // if connection failed
        if (connected_socket_ == INVALID_SOCKET)
        {
            printf("Unable to connect to server!\n");
            system("PAUSE");
            WSACleanup();
            return;
        }


        // Set the mode of the socket to be nonblocking
        u_long mode = 1;

        result_ = ioctlsocket(connected_socket_, FIONBIO, &mode);
        if (result_ == SOCKET_ERROR)
        {
            printf("ioctlsocket failed with error: %d\n", WSAGetLastError());
            system("PAUSE");
            closesocket(connected_socket_);
            WSACleanup();
            return;
        }
    }

    int ClientNetwork::ReceivePackets(char* recvbuf)
    {
        result_ = network::services::ReceiveMessage(connected_socket_, recvbuf, kMaxPacketSize);

        if (result_ == 0)
        {
            printf("Connection closed\n");
            system("PAUSE");
            closesocket(connected_socket_);
            WSACleanup();
            exit(1);
        }

        return result_;
    }

}