#pragma once

#include <vector>
#include <chrono>

#include "server_network.h"
#include "network_data.h"
#include "../info/configs.h"
#include "../info/player.h"


class ServerGame
{
public:
    Configs* configs;

    // Real-time map with the players
    std::vector<std::vector<char>> real_map;

    // players number and player
    std::map<size_t, Player*> players_locations;

    // players number and last firing time
    std::map<size_t, std::chrono::system_clock::time_point> last_firing_times;

    ServerGame(Configs*);
    ~ServerGame(void);

    void update();

    // Printing real-time map
    void printMap();


private:
    // IDs for the clients connecting for table in ServerNetwork 
    static unsigned int client_id;

    // The ServerNetwork object 
    ServerNetwork* network;

    // data buffer
    char network_data[MAX_PACKET_SIZE];

    // Receive all locations
    void receiveFromClients();

    // Sends location of player to all players
    void sendMovementPackets(Player* player);

    // Sends shouting information: decreasing of health and small movement && changes target info
    void sendShootingInfo(Player* shooter, Player* player_to_change);

    // Send Configs to the client
    void sendConfigs(size_t client);

    // Send Configs to the client
    void sendMap(size_t client);

    // Updates player's position on the map
    void updateMap(Player*);

    // Tests whether there is a hit
    bool checkShouting(Player* shooter, Player* target);

    // get crossing point
    bool cross(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float& x, float& y);
};