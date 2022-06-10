#pragma once

#include "ServerNetwork.h"
#include "Configs.h"
#include "NetworkData.h"
#include "Player.h"
#include <vector>


class ServerGame
{
public:
    Configs* configs;

    // Real-time map with the players
    std::vector<std::vector<char>> real_map;

    // players number and player
    std::map<size_t, Player*> players_locations;
    
    ServerGame(Configs*);
    ~ServerGame(void);

    void update();

    // Printing real-time map
    void printMap();


private:

    // Receive all locations
    void receiveFromClients();

    // Sends location of player to all players
    void sendMovementPackets(Player* player);

    // Sends shouting information: decreasing of health and small movement && changes target info
    void sendShoutingInfo(Player *shooter, Player* player_to_change);

    // Send Configs to the client
    void sendConfigs(size_t client);

    // IDs for the clients connecting for table in ServerNetwork 
    static unsigned int client_id;

    // The ServerNetwork object 
    ServerNetwork* network;

    // data buffer
    char network_data[MAX_PACKET_SIZE];

    // Updates player's position on the map
    void updateMap(Player*);

    // Tests whether there is a hit
    bool checkShouting(Player* shooter, Player* target);

    // get crossing point
    bool cross(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float& x, float& y);
};