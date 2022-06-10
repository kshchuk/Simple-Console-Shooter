#include "ServerGame.h"
#include <iostream>
#include <string>

unsigned int ServerGame::client_id;

ServerGame::ServerGame(Configs* conf)
{
    // assign game configs (map, speed, etc)
    configs = conf;

    // id's to assign clients for our table
    client_id = 0;

    // set up the server network to listen 
    network = new ServerNetwork();

    real_map.resize(conf->mapHeight);
    for (int i = 0; i < conf->mapHeight; i++) {
        real_map[i].resize(conf->mapWidth);
        for (int j = 0; j < conf->mapWidth; j++)
            real_map[i][j] = conf->map[i][j] ? '#' : ' ';
    }
}

void ServerGame::update()
{
    // get new clients
    if (network->acceptNewClient(client_id))
    {
        printf("Client %d has been connected to the server\n", client_id);

        // Send configs to the client
        sendConfigs(client_id);

        Player* p = new Player(*configs);
        p->index = client_id;

        players_locations.insert(std::make_pair(client_id, p));

        client_id++;
    }
    receiveFromClients();
}

void ServerGame::sendMovementPackets(Player* player)
{
    // send index and coordinates of the player to other players

    Packet packet;
    packet.packet_type = MOVEMENT;

    packet.size_of_packet_data = sizeof(player->index) + sizeof(player->x) + sizeof(player->y) + sizeof(player->rotation);
    packet.packet_data = new char[packet.size_of_packet_data];

    memcpy(packet.packet_data, &player->index, sizeof(player->index));
    memcpy(packet.packet_data + sizeof(player->index), &player->x, sizeof(player->x));
    memcpy(packet.packet_data + sizeof(player->index) + sizeof(player->x), &player->y, sizeof(player->y));
    memcpy(packet.packet_data + sizeof(player->index) + sizeof(player->x) + sizeof(player->y), &player->rotation, sizeof(player->rotation));

    size_t packet_size = packet.size_of_packet_data + sizeof(packet.packet_type) + sizeof(packet.packet_data);
    char* compressed_packet = new char[packet_size];

    packet.serialize(compressed_packet);

    std::map<unsigned int, SOCKET>::iterator iter;
    for (iter = network->sessions.begin(); iter != network->sessions.end(); iter++)
    {
        int client_index = (*iter).first;

        // In case we don't need to send data to the same client
        if (client_index == player->index)
            continue;
        else 
        {
            NetworkServices::sendMessage((*iter).second, compressed_packet, packet_size);
        }
    }

    delete[] compressed_packet;
    delete[] packet.packet_data;

}
void ServerGame::printMap()
{
    system("cls");
    for (auto& line : real_map) {
        printf("\n");
        for (auto& elem : line)
            printf("%c", elem);
    }
}

void ServerGame::sendConfigs(size_t client)
{
    // Convert map to single array
    size_t map_size = configs->mapWidth * configs->mapHeight;
    int* map = new int[map_size];
    for (size_t i = 0; i < configs->mapHeight; i++)
        for (size_t j = 0; j < configs->mapWidth; j++) {
            map[i * configs->mapWidth + j] = (int)configs->map[i][j];
        }

    Packet packet;
    packet.packet_type = INIT_CONNECTION;
    packet.size_of_packet_data = sizeof(Configs) + map_size*sizeof(int);

    packet.packet_data = new char[packet.size_of_packet_data];
    memcpy(packet.packet_data, configs, sizeof(Configs));
    memcpy(packet.packet_data + sizeof(Configs), map, map_size*sizeof(int));

    size_t packet_size = sizeof(packet.packet_type) + sizeof(packet.size_of_packet_data) + packet.size_of_packet_data;
    char* packet_data = new char[packet_size];
    
    packet.serialize(packet_data);
    
    NetworkServices::sendMessage(network->sessions[client], packet_data, packet_size);

    printf("Configuration file sended\n");

    delete[] map;
    delete[] packet_data;
    delete[] packet.packet_data;
}

void ServerGame::updateMap(Player* player)
{
    // Replace player index in the map
    for (size_t i = 0; i < real_map.size(); i++)
        for (size_t j = 0; j < real_map[i].size(); j++)
            if (real_map[i][j] == player->index + '0') {
                real_map[i][j] = configs->map[i][j] ? '#' : ' ';
                real_map[(int)player->x][(int)player->y] = player->index + '0';
                return;
            }
    // if not found - add player to map
    real_map[(int)player->x][(int)player->y] = player->index + '0';
}

void ServerGame::receiveFromClients()
{
    Packet packet;

    // go through all clients
    std::map<unsigned int, SOCKET>::iterator iter;

    for (iter = network->sessions.begin(); iter != network->sessions.end(); iter++)
    {
        int data_length = network->receiveData(iter->first, network_data);

        if (data_length <= 0)
        {
            //no data recieved
            continue;
        }

        int i = 0;
        while (i < (unsigned int)data_length)
        {
            packet.deserialize(&(network_data[i]));
            i += sizeof(packet.packet_type) + sizeof(packet.size_of_packet_data) + packet.size_of_packet_data;

            switch (packet.packet_type) {
                
                // Receiving location: x, y, rotation
            case MOVEMENT:
            {
                // Update player locations
                size_t player_index = (*iter).first;
                Player* player_to_change = players_locations[player_index];
                memcpy(&player_to_change->x, packet.packet_data, sizeof(player_to_change->x));
                memcpy(&player_to_change->y, packet.packet_data + sizeof(player_to_change->x), sizeof(player_to_change->y));
                memcpy(&player_to_change->rotation, packet.packet_data + sizeof(player_to_change->x) + sizeof(player_to_change->y),
                    sizeof(player_to_change->rotation));
                
                // Update the real map
                updateMap(player_to_change);

                // Resend information to other clients
                sendMovementPackets(players_locations[player_index]);
                delete[] packet.packet_data;
                break;
            }
            case SHOUTING:
            {
                size_t player_index = (*iter).first;
                sendShoutingInfo(player_index);
            }
                break;
//
            default:
//
//                printf("error in packet types\n");
//
               break;
            }
     }
  }
}