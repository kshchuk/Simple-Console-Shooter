#include "ClientGame.h" 
#include "NetworkData.h"


ClientGame::ClientGame(char* SERVER_IP, bool isMultiplayer)
{
    if (isMultiplayer)
        network = new ClientNetwork(SERVER_IP);
}

// Receive packets
void ClientGame::update()
{
    if (this->player)
        sendPlayerLocation();

    Packet packet;
    int data_length = network->receivePackets(network_data);

    if (data_length <= 0)
    {
        //no data recieved
        return;
    }

    int i = 0;
    while (i < (unsigned int)data_length)
    {
        packet.deserialize(&(network_data[i]));
        i += sizeof(packet.packet_type) + sizeof(packet.size_of_packet_data) + packet.size_of_packet_data;

        switch (packet.packet_type)
        {
        case INIT_CONNECTION:
        {
            config = new Configs(packet.packet_data);
            printf("Client received configuration file\n");
            gotConfigs = true;
            break;
        }
        case MOVEMENT:
        {
            int player_index;
            memcpy(&player_index, packet.packet_data, sizeof(player_index));

            // If there's no such player before
            if (other_players.count(player_index) == 0)
                other_players.insert(std::make_pair(player_index, new Player(*config)));

            Player* player_to_change = other_players[player_index];

            player_to_change->index = player_index;

            memcpy(&player_to_change->x, packet.packet_data + sizeof(player_to_change->index), sizeof(player_to_change->x));
            memcpy(&player_to_change->y, packet.packet_data + sizeof(player_to_change->index) + sizeof(player_to_change->x), sizeof(player_to_change->y));
            memcpy(&player_to_change->rotation, packet.packet_data + sizeof(player_to_change->index) + sizeof(player_to_change->x) +
                sizeof(player_to_change->y), sizeof(player_to_change->rotation));

            break;
        }
        case SHOOTING:
        {
            memcpy(&player->health, packet.packet_data, sizeof(player->health));
            memcpy(&player->x, packet.packet_data + sizeof(player->health), sizeof(player->x));
            memcpy(&player->y, packet.packet_data + sizeof(player->health) + sizeof(player->x), sizeof(player->y));
            memcpy(&player->rotation, packet.packet_data + sizeof(player->health) + sizeof(player->x) + sizeof(player->y), sizeof(player->rotation));
            
            break;
        }
        default:
            break;
        }
    }
}

void ClientGame::sendPlayerLocation()
{
    Packet packet;
    packet.packet_type = MOVEMENT;
    
    packet.size_of_packet_data = sizeof(player->x) + sizeof(player->y) + sizeof(player->rotation);
    packet.packet_data = new char[packet.size_of_packet_data];

    memcpy(packet.packet_data, &player->x, sizeof(player->x));
    memcpy(packet.packet_data + sizeof(player->x), &player->y, sizeof(player->y));
    memcpy(packet.packet_data + sizeof(player->x) + sizeof(player->y), &player->rotation, sizeof(player->rotation));

    size_t packed_size = packet.size_of_packet_data + sizeof(packet.packet_type) + sizeof(packet.packet_data);
    char* compressed_packet = new char[packed_size];
    packet.serialize(compressed_packet);

    NetworkServices::sendMessage(network->ConnectSocket, compressed_packet, packed_size);

    delete[] packet.packet_data;
    delete[] compressed_packet;
}


// Send location && direction of shooting
void ClientGame::sendShootingInfo()
{
    Packet packet;
    packet.packet_type = SHOOTING;
    
    packet.size_of_packet_data = sizeof(player->x) + sizeof(player->y) + sizeof(player->rotation);
    packet.packet_data = new char[packet.size_of_packet_data];

    memcpy(packet.packet_data, &player->x, sizeof(player->x));
    memcpy(packet.packet_data + sizeof(player->x), &player->y, sizeof(player->y));
    memcpy(packet.packet_data + sizeof(player->x) + sizeof(player->y), &player->rotation, sizeof(player->rotation));

    size_t packed_size = packet.size_of_packet_data + sizeof(packet.packet_type) + sizeof(packet.packet_data);
    char* compressed_packet = new char[packed_size];
    packet.serialize(compressed_packet);

    NetworkServices::sendMessage(network->ConnectSocket, compressed_packet, packed_size);

    delete[] packet.packet_data;
    delete[] compressed_packet;
}

void ClientGame::RegisterPlayer(Player& player)
{
    this->player = &player;
}
