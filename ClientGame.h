#pragma once
#include <winsock2.h>
#include <Windows.h>
#include "ClientNetwork.h"
#include "Configs.h"


class ClientGame
{
public:
    Configs* config;

    ClientGame();
    ~ClientGame(void);

    ClientNetwork* network;

    //void sendActionPackets();

    char network_data[MAX_PACKET_SIZE];

    void update();
};
