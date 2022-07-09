#pragma once

#include <winsock2.h>
#include <Windows.h>

#include <map>
#include <string>

#include "client_network.h"
#include "../info/configs.h"
#include "../info/player.h"

namespace network
{
    class ClientGame
    {
    public:
        Configs* configs_;

        Player* player_ = nullptr;

        // Index and player
        std::map<int, Player*> other_players_;

        bool got_configs_ = false;
        bool got_map_ = false;

        ClientNetwork* network_;

        char network_data_[kMaxPacketSize];

        ClientGame(Configs* configs);

        // Receive packets
        void update();

        // Send location: x, y, rotation
        void SendPlayerLocation();

        // Send location && direction of shooting
        void SendShootingInfo();

        void RegisterPlayer(Player* player);
    };

}