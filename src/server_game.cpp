/*
 *
 * File: server_game.cpp
 *
 * Author: Yaroslav Kishchuk
 * Contact: kshchuk@gmail.com
 *
 */

#include "../include/network/server_game.h"

#include <iostream>
#include <string>

#include "../include/network/network_data.h"
#include "../include/info/configs.h"
#include "../include/info/player.h"


namespace network
{
    unsigned int ServerGame::client_id_;

    ServerGame::ServerGame()
    {
        // Id's to assign clients for our table
        client_id_ = 0;

        // Set up the server network to listen 
        network_ = new ServerNetwork(Configs::default_port);

        real_map_.resize(Configs::map_height);
        for (int i = 0; i < Configs::map_height; i++) {
            real_map_[i].resize(Configs::map_width);
            for (int j = 0; j < Configs::map_width; j++)
                real_map_[i][j] = Configs::map[i][j] ? '#' : ' ';
        }
    }

    void ServerGame::update()
    {
        // Get new clients
        if (network_->AcceptNewClient(client_id_))
        {
            printf("Client %d has been connected to the server\n", client_id_);

            // Send configs to the client
            SendConfigs(client_id_);

            SendMap(client_id_);

            Player* p = new Player();
            p->index = client_id_;

            players_locations_.insert(std::make_pair(client_id_, p));
            last_firing_times_.insert(std::make_pair(client_id_, std::chrono::system_clock::now()));

            client_id_++;
        }
        ReceiveFromClients();
    }

    void ServerGame::SendMovementPackets(Player* player)
    {
        // Send index and coordinates of the player to other players

        Packet packet;
        packet.packet_type = kMovement;

        packet.size_of_packet_data = sizeof(player->index) + sizeof(player->pos.x) + sizeof(player->pos.y) + sizeof(player->rotation);
        packet.packet_data = new char[packet.size_of_packet_data];

        memcpy(packet.packet_data, &player->index, sizeof(player->index));
        memcpy(packet.packet_data + sizeof(player->index), &player->pos.x, sizeof(player->pos.x));
        memcpy(packet.packet_data + sizeof(player->index) + sizeof(player->pos.x), &player->pos.y, sizeof(player->pos.y));
        memcpy(packet.packet_data + sizeof(player->index) + sizeof(player->pos.x) + sizeof(player->pos.y), &player->rotation, sizeof(player->rotation));

        size_t packet_size = packet.size_of_packet_data + sizeof(packet.packet_type) + sizeof(packet.packet_data);
        char* compressed_packet = new char[packet_size];

        packet.serialize(compressed_packet);

        std::map<unsigned int, SOCKET>::iterator iter;
        for (iter = network_->sessions_.begin(); iter != network_->sessions_.end(); iter++)
        {
            int client_index = (*iter).first;

            // In case we don't need to send data to the same client
            if (client_index == player->index)
                continue;
            else
            {
                network::services::SendMessage((*iter).second, compressed_packet, packet_size);
            }
        }

        delete[] compressed_packet;
        delete[] packet.packet_data;

    }

    // Sends shouting information: new health and small movement (0.05) or return player to spawn
    void ServerGame::SendShootingInfo(Player* shooter, Player* player_to_change)
    {
        if (player_to_change->health - Configs::shooting_damage <= 0) {
            player_to_change->RandomPosition(); // respawn
            player_to_change->health = Configs::health;
            printf("Player %i killed player %i\n", shooter->index, player_to_change->index);
        }
        else {
            player_to_change->health -= Configs::shooting_damage;

            // Do some moving of the target
            float direct_x = sinf(shooter->rotation);   // Unit vector for ray in shooting direction
            float direct_y = cosf(shooter->rotation);

            player_to_change->pos.x += direct_x * 0.05;
            player_to_change->pos.y += direct_y * 0.05;
        }

        Packet packet;
        packet.packet_type = kShooting;
        packet.size_of_packet_data = sizeof(player_to_change->health) + sizeof(player_to_change->pos.x) +
            sizeof(player_to_change->pos.y) + sizeof(player_to_change->rotation);

        packet.packet_data = new char[packet.size_of_packet_data];
        memcpy(packet.packet_data, &player_to_change->health, sizeof(player_to_change->health));
        memcpy(packet.packet_data + sizeof(player_to_change->health), &player_to_change->pos.x, sizeof(player_to_change->pos.x));
        memcpy(packet.packet_data + sizeof(player_to_change->health) + sizeof(player_to_change->pos.x),
            &player_to_change->pos.y, sizeof(player_to_change->pos.y));
        memcpy(packet.packet_data + sizeof(player_to_change->health) + sizeof(player_to_change->pos.x) + sizeof(player_to_change->pos.y),
            &player_to_change->rotation, sizeof(player_to_change->rotation));

        size_t packet_size = sizeof(packet.packet_type) + sizeof(packet.size_of_packet_data) + packet.size_of_packet_data;
        char* packet_data = new char[packet_size];

        packet.serialize(packet_data);

        network::services::SendMessage(network_->sessions_[player_to_change->index], packet_data, packet_size);

        delete[] packet.packet_data;
        delete[] packet_data;
    }
    void ServerGame::PrintMap()
    {
        system("cls");
        for (auto& line : real_map_) {
            printf("\n");
            for (auto& elem : line)
                printf("%c", elem);
        }
    }

    void ServerGame::SendConfigs(size_t client)
    {
        Packet packet;
        packet.packet_type = kInitConnetion;
        packet.size_of_packet_data = sizeof(Configs);

        packet.packet_data = new char[packet.size_of_packet_data];
        memcpy(packet.packet_data, new Configs, sizeof(Configs));

        size_t packet_size = sizeof(packet.packet_type) + sizeof(packet.size_of_packet_data) + packet.size_of_packet_data;
        char* packet_data = new char[packet_size];

        packet.serialize(packet_data);

        network::services::SendMessage(network_->sessions_[client], packet_data, packet_size);

        printf("Configuration file sent\n");

        delete[] packet_data;
        delete[] packet.packet_data;
    }

    void ServerGame::SendMap(size_t client)
    {
        // Convert map to single array
        size_t map_size = Configs::map_width * Configs::map_height;
        bool* map = new bool[map_size];
        for (size_t i = 0; i < Configs::map_height; i++)
            for (size_t j = 0; j < Configs::map_width; j++) {
                map[i * Configs::map_width + j] = Configs::map[i][j];
            }

        Packet packet;
        packet.packet_type = kMap;
        packet.size_of_packet_data = map_size;

        packet.packet_data = new char[packet.size_of_packet_data];
        memcpy(packet.packet_data, map, map_size);

        size_t packet_size = sizeof(packet.packet_type) + sizeof(packet.size_of_packet_data) + packet.size_of_packet_data;
        char* packet_data = new char[packet_size];

        packet.serialize(packet_data);

        network::services::SendMessage(network_->sessions_[client], packet_data, packet_size);

        printf("Map sent\n");

        delete[] packet_data;
        delete[] packet.packet_data;
    }

    void ServerGame::UpdateMap(Player* player)
    {
        // Replace player index in the map
        for (size_t i = 0; i < real_map_.size(); i++)
            for (size_t j = 0; j < real_map_[i].size(); j++)
                if (real_map_[i][j] == player->index + '0') {
                    real_map_[i][j] = Configs::map[i][j] ? '#' : ' ';
                    real_map_[(int)player->pos.x][(int)player->pos.y] = player->index + '0';
                    return;
                }
        // if not found - add player to map
        real_map_[static_cast<int> (player->pos.x)][static_cast<int> (player->pos.y)] = 
                                                                        player->index + '0';
    }

    bool ServerGame::CheckShouting(const Player* shooter, Player* target)
    {
        target->CaclulatePositions();

        bool hit_player = false;		// Set when ray hits player silhouette
        bool hit_wall = false;			// Set when ray hits wall block

        float direct_vector_x = sinf(shooter->rotation);   // Unit vector for ray in player space
        float direct_vector_y = cosf(shooter->rotation);

        // Crosing point
        float cur_ray_pos_x = shooter->pos.x + direct_vector_x * Configs::max_shooting_range;
        float cur_ray_pos_y = shooter->pos.y + direct_vector_y * Configs::max_shooting_range;

        // Check whether the ray crosses the silhoete
        if (cross(shooter->pos.x, shooter->pos.y, cur_ray_pos_x, cur_ray_pos_y,
            target->left_front.x, target->left_front.y,
            target->right_front.x, target->right_front.y, cur_ray_pos_x, cur_ray_pos_y) ||
            cross(shooter->pos.x, shooter->pos.y, cur_ray_pos_x, cur_ray_pos_y,
                target->left_front.x, target->left_front.y,
                target->left_back.x, target->left_back.y, cur_ray_pos_x, cur_ray_pos_y) ||
            cross(shooter->pos.x, shooter->pos.y, cur_ray_pos_x, cur_ray_pos_y,
                target->left_back.x, target->left_back.y,
                target->right_back.x, target->right_back.y, cur_ray_pos_x, cur_ray_pos_y) ||
            cross(shooter->pos.x, shooter->pos.y, cur_ray_pos_x, cur_ray_pos_y,
                target->right_back.x, target->right_back.y,
                target->right_front.x, target->right_front.y, cur_ray_pos_x, cur_ray_pos_y))
        {
            float step_size = 0.1f;		             // Increment size for ray casting, decrease to increase resolution						
            float distance_to_obstacle = 0.0f;       //	

            // Incrementally cast ray from player, along ray angle, testing for 
            // intersection with a block or a player
            while (!hit_wall && !hit_player && distance_to_obstacle < Configs::max_shooting_range)
            {
                distance_to_obstacle += step_size;
                cur_ray_pos_x = shooter->pos.x + direct_vector_x * distance_to_obstacle;
                cur_ray_pos_y = shooter->pos.y + direct_vector_y * distance_to_obstacle;

                // Test if ray is out of bounds
                if (cur_ray_pos_x < 0 || cur_ray_pos_x >= Configs::map_width || cur_ray_pos_y < 0 || cur_ray_pos_y >= Configs::map_height)
                {
                    hit_wall = true;
                }
                else // Ray is inbounds so test to see if the ray cell is a wall block
                    if (Configs::map[static_cast<int> (cur_ray_pos_x)][static_cast<int> (cur_ray_pos_y)])
                    {
                        // Ray has hit wall
                        hit_wall = true;
                    }
                    else
                        if (cross(shooter->pos.x, shooter->pos.y, cur_ray_pos_x, cur_ray_pos_y,
                                target->left_front.x, target->left_front.y,
                                target->right_front.x, target->right_front.y, cur_ray_pos_x, cur_ray_pos_y) ||
                            cross(shooter->pos.x, shooter->pos.y, cur_ray_pos_x, cur_ray_pos_y,
                                target->left_front.x, target->left_front.y,
                                target->left_back.x, target->left_back.y, cur_ray_pos_x, cur_ray_pos_y) ||
                            cross(shooter->pos.x, shooter->pos.y, cur_ray_pos_x, cur_ray_pos_y,
                                target->left_back.x, target->left_back.y,
                                target->right_back.x, target->right_back.y, cur_ray_pos_x, cur_ray_pos_y) ||
                            cross(shooter->pos.x, shooter->pos.y, cur_ray_pos_x, cur_ray_pos_y,
                                target->right_back.x, target->right_back.y,
                                target->right_front.x, target->right_front.y, cur_ray_pos_x, cur_ray_pos_y))
                        {
                            hit_player = true;
                        }
            }
        }
        return hit_player;
    }

    void ServerGame::ReceiveFromClients()
    {
        Packet packet;

        // Go through all clients
        std::map<unsigned int, SOCKET>::iterator iter;

        for (iter = network_->sessions_.begin(); iter != network_->sessions_.end(); iter++)
        {
            int data_length = network_->ReceiveData(iter->first, network_data_);

            if (data_length <= 0)
            {
                // No data recieved
                continue;
            }

            int i = 0;
            while (i < static_cast<unsigned int> (data_length))
            {
                packet.deserialize(&(network_data_[i]));
                i += sizeof(packet.packet_type) + sizeof(packet.size_of_packet_data) + packet.size_of_packet_data;

                switch (packet.packet_type) {

                    // Receiving location: x, y, rotation
                case kMovement:
                {
                    // Update player location
                    size_t player_index = (*iter).first;
                    Player* player_to_change = players_locations_[player_index];
                    memcpy(&player_to_change->pos.x, packet.packet_data, sizeof(player_to_change->pos.x));
                    memcpy(&player_to_change->pos.y, packet.packet_data + sizeof(player_to_change->pos.x), sizeof(player_to_change->pos.y));
                    memcpy(&player_to_change->rotation, packet.packet_data + sizeof(player_to_change->pos.x) + sizeof(player_to_change->pos.y),
                        sizeof(player_to_change->rotation));

                    // Update the real map
                    UpdateMap(player_to_change);

                    // Send information to other clients
                    SendMovementPackets(players_locations_[player_index]);

                    delete[] packet.packet_data;
                    break;
                }
                case kShooting:
                {
                    // Update player location
                    size_t player_index = (*iter).first;

                    // Check is reloaded
                    std::chrono::system_clock::now();
                    std::chrono::duration<float> difference = std::chrono::system_clock::now() - last_firing_times_[player_index];
                    if (difference.count() < Configs::gun_reloading)
                        break;
                    else
                        last_firing_times_[player_index] = std::chrono::system_clock::now();

                    Player* shooter = players_locations_[player_index];
                    memcpy(&shooter->pos.x, packet.packet_data, sizeof(shooter->pos.x));
                    memcpy(&shooter->pos.y, packet.packet_data + sizeof(shooter->pos.x), sizeof(shooter->pos.y));
                    memcpy(&shooter->rotation, packet.packet_data + sizeof(shooter->pos.x) + sizeof(shooter->pos.y),
                        sizeof(shooter->rotation));

                    // Checks all players for a hit
                    for (auto iter : players_locations_)
                    {
                        if (CheckShouting(shooter, iter.second) && shooter->index != iter.first)
                        {
                            SendShootingInfo(shooter, iter.second);
                            SendMovementPackets(iter.second);
                            UpdateMap(iter.second);
                        }
                    }

                    delete[] packet.packet_data;
                    break;
                }
                //
                default:                
                    printf("error in packet types\n");
                    break;
                }
            }
        }
    }

    bool ServerGame::cross(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float& x, float& y)
    {
        float Ua, Ub, numerator_a, numerator_b, denominator;

        denominator = (y4 - y3) * (x1 - x2) - (x4 - x3) * (y1 - y2);

        if (denominator == 0) {
            return false;
        }
        else {
            numerator_a = (x4 - x2) * (y4 - y3) - (x4 - x3) * (y4 - y2);
            numerator_b = (x1 - x2) * (y4 - y2) - (x4 - x2) * (y1 - y2);
            Ua = numerator_a / denominator;
            Ub = numerator_b / denominator;

            if (Ua >= 0 && Ua <= 1 && Ub >= 0 && Ub <= 1) {
                x = x1 + x2 * Ua;
                y = y1 + y2 * Ua;
                return true;
            }
            else
                return false;
        }
    }
}