/*
 *
 * File: server_game.h
 *
 * Author: Yaroslav Kishchuk
 * Contact: kshchuk@gmail.com
 *
 */


#pragma once

#include <vector>
#include <chrono>

#include "server_network.h"
#include "network_data.h"
#include "../info/configs.h"
#include "../info/player.h"


namespace network
{
    class ServerGame
    {
    public:
        Configs* configs_;

        // Real-time map with the players
        std::vector<std::vector<char>> real_map_;

        // Players index and player
        std::map<size_t, Player*> players_locations_;

        // Player's index and last firing time
        std::map<size_t, std::chrono::system_clock::time_point> last_firing_times_;

        ServerGame(Configs*);
        ~ServerGame(void);

        // Gets information from clients, register new clients, 
        // checks shooting
        void update();

        // Prints real-time map
        void PrintMap();


    private:
        // IDs for the clients connecting for table in ServerNetwork 
        static unsigned int client_id_;

        // The ServerNetwork object 
        ServerNetwork* network_;

        // Data buffer
        char network_data_[kMaxPacketSize];

        // Receives all locations
        void ReceiveFromClients();

        // Sends location of player to all players
        void SendMovementPackets(Player* player);

        // Sends shouting information: decreasing of health and small movement && changes target info
        void SendShootingInfo(Player* shooter, Player* player_to_change_info);

        // Sends Configs to the client
        void SendConfigs(size_t client);

        // Sends Configs to the client
        void SendMap(size_t client);

        // Updates player's position on the map
        void UpdateMap(Player*);

        // Tests whether there is a hit
        bool CheckShouting(Player* shooter, Player* target);

        // Gets crossing point
        bool cross(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float& x, float& y);
    };
}