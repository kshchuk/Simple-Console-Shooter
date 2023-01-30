/*
 *
 * File: client_game.h
 *
 * Author: Yaroslav Kishchuk
 * Contact: kshchuk@gmail.com
 *
 */


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
    // Stores information and runs multiplayer game.
    class ClientGame
    {
    public:

        // Me
        Player* player_ = nullptr;

        // Is used to render other players on screen.
        // Index and Player.
        std::map<int, Player*> other_players_;

        bool got_configs_ = false;
        bool got_map_ = false;

        // Connection with server.
        ClientNetwork* network_ = nullptr;

        char network_data_[kMaxPacketSize];

        ClientGame();

        // Receives other players' location, current state of the player(
        // i.e. myself) and sends my info
        void update();

        // Sends location: x, y, rotation to the server.
        void SendPlayerLocation();

        // Sends location and direction of shooting to the server.
        void SendShootingInfo();

        // Register myself in the ClientGame class.
        void RegisterPlayer(Player* player);
    };

}