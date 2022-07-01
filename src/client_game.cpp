#include "../include/network/client_game.h"
#include "../include/network/network_data.h"

namespace network
{

    ClientGame::ClientGame(Configs* configs)
    {
        configs_ = configs;
        network_ = new ClientNetwork(configs_->server_ip, configs_->default_port);
    }

    // Receive packets
    void ClientGame::update()
    {
        if (player_)
            SendPlayerLocation();

        Packet packet;
        int data_length = network_->ReceivePackets(network_data_);

        if (data_length <= 0)
        {
            //no data recieved
            return;
        }

        int i = 0;
        while (i < static_cast<unsigned int> (data_length))
        {
            packet.deserialize(&(network_data_[i]));
            i += sizeof(packet.packet_type) + sizeof(packet.size_of_packet_data) + packet.size_of_packet_data;

            switch (packet.packet_type)
            {
            case kInitConnetion:
            {
                configs_ = new Configs(packet.packet_data);
                printf("Client received configuration file\n");
                got_configs_ = true;
                break;
            }
            case kMap:
            {
                configs_->GetMap(packet.packet_data);
                printf("Client received map\n");
                got_map_ = true;
                break;
            }
            case kMovement:
            {
                int player_index;
                memcpy(&player_index, packet.packet_data, sizeof(player_index));

                // If there's no such player before
                if (other_players_.count(player_index) == 0)
                    other_players_.insert(std::make_pair(player_index, new Player(*configs_)));

                Player* player_to_change = other_players_[player_index];

                player_to_change->index = player_index;

                memcpy(&player_to_change->pos.x, packet.packet_data + sizeof(player_to_change->index), sizeof(player_to_change->pos.x));
                memcpy(&player_to_change->pos.y, packet.packet_data + sizeof(player_to_change->index) + sizeof(player_to_change->pos.x), sizeof(player_to_change->pos.y));
                memcpy(&player_to_change->rotation, packet.packet_data + sizeof(player_to_change->index) + sizeof(player_to_change->pos.x) +
                    sizeof(player_to_change->pos.y), sizeof(player_to_change->rotation));

                break;
            }
            case kShooting:
            {
                memcpy(&player_->health, packet.packet_data, sizeof(player_->health));
                memcpy(&player_->pos.x, packet.packet_data + sizeof(player_->health), sizeof(player_->pos.x));
                memcpy(&player_->pos.y, packet.packet_data + sizeof(player_->health) + sizeof(player_->pos.x), sizeof(player_->pos.y));
                memcpy(&player_->rotation, packet.packet_data + sizeof(player_->health) + sizeof(player_->pos.x) + sizeof(player_->pos.y), sizeof(player_->rotation));

                break;
            }
            default:
                break;
            }
        }
    }

    void ClientGame::SendPlayerLocation()
    {
        Packet packet;
        packet.packet_type = kMovement;

        packet.size_of_packet_data = sizeof(player_->pos.x) + sizeof(player_->pos.y) + sizeof(player_->rotation);
        packet.packet_data = new char[packet.size_of_packet_data];

        memcpy(packet.packet_data, &player_->pos.x, sizeof(player_->pos.x));
        memcpy(packet.packet_data + sizeof(player_->pos.x), &player_->pos.y, sizeof(player_->pos.y));
        memcpy(packet.packet_data + sizeof(player_->pos.x) + sizeof(player_->pos.y), &player_->rotation, sizeof(player_->rotation));

        size_t packed_size = packet.size_of_packet_data + sizeof(packet.packet_type) + sizeof(packet.packet_data);
        char* compressed_packet = new char[packed_size];
        packet.serialize(compressed_packet);

        network::services::SendMessage(network_->connected_socket_, compressed_packet, packed_size);

        delete[] packet.packet_data;
        delete[] compressed_packet;
    }


    // Send location && direction of shooting
    void ClientGame::SendShootingInfo()
    {
        Packet packet;
        packet.packet_type = kShooting;

        packet.size_of_packet_data = sizeof(player_->pos.x) + sizeof(player_->pos.y) + sizeof(player_->rotation);
        packet.packet_data = new char[packet.size_of_packet_data];

        memcpy(packet.packet_data, &player_->pos.x, sizeof(player_->pos.x));
        memcpy(packet.packet_data + sizeof(player_->pos.x), &player_->pos.y, sizeof(player_->pos.y));
        memcpy(packet.packet_data + sizeof(player_->pos.x) + sizeof(player_->pos.y), &player_->rotation, sizeof(player_->rotation));

        size_t packed_size = packet.size_of_packet_data + sizeof(packet.packet_type) + sizeof(packet.packet_data);
        char* compressed_packet = new char[packed_size];
        packet.serialize(compressed_packet);

        network::services::SendMessage(network_->connected_socket_, compressed_packet, packed_size);

        delete[] packet.packet_data;
        delete[] compressed_packet;
    }

    void ClientGame::RegisterPlayer(Player* player)
    {
        player_ = player;
    }
}