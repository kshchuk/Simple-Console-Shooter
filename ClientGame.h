#pragma once
#include <winsock2.h>
#include <Windows.h>
#include <map>
#include "ClientNetwork.h"
#include "Configs.h"
#include "Player.h"


class ClientGame
{
public:
    Configs* config;

    Player* player = nullptr;

    // Index and player
    std::map<int, Player*> other_players;

    ClientGame(char* SERVER_IP);
    ~ClientGame(void);

    bool gotConfigs = false;

    ClientNetwork* network;

    char network_data[MAX_PACKET_SIZE];

    // Receive packets
    void update();

    // Send location: x, y, rotation
    void sendPlayerLocation();

    // Send location && direction of shooting
    void sendShootingInfo();

    void RegisterPlayer(Player& player);
};
