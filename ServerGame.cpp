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

bool ServerGame::checkShouting(Player* shooter, Player* target)
{
    target->CaclulatePositions();

    bool bHitPlayer = false;		// Set when ray hits player silhouette
	bool bHitWall = false;			// Set when ray hits wall block

    float fEyeX = sinf(shooter->rotation);   // Unit vector for ray in player space
    float fEyeY = cosf(shooter->rotation);

    // Crosing point
    float fTestX = shooter->x + fEyeX * configs->maxShootingRange;
	float fTestY = shooter->y + fEyeY * configs->maxShootingRange;
    
    // Check whether the ray crosses the silhoete
	if (cross(shooter->x, shooter->y, fTestX, fTestY,
			target->leftFront_pos.x, target->leftFront_pos.y,
			target->rightFront_pos.x, target->rightFront_pos.y, fTestX, fTestY) ||
		cross(shooter->x, shooter->y, fTestX, fTestY,
			target->leftFront_pos.x, target->leftFront_pos.y,
			target->leftBack_pos.x, target->leftBack_pos.y, fTestX, fTestY) ||
		cross(shooter->x, shooter->y, fTestX, fTestY,
			target->leftBack_pos.x, target->leftBack_pos.y,
			target->rightBack_pos.x, target->rightBack_pos.y, fTestX, fTestY) ||
		cross(shooter->x, shooter->y, fTestX, fTestY,
			target->rightBack_pos.x, target->rightBack_pos.y,
			target->rightFront_pos.x, target->rightFront_pos.y, fTestX, fTestY))
	{
        float fStepSize = 0.1f;		            // Increment size for ray casting, decrease to increase resolution						
		float fDistanceToObstacle = 0.0f;       //	

        // Incrementally cast ray from player, along ray angle, testing for 
		// intersection with a block or a player
        while (!bHitWall && !bHitPlayer && fDistanceToObstacle < configs->maxShootingRange)
        {
            fDistanceToObstacle += fStepSize;
		    fTestX = shooter->x + fEyeX * fDistanceToObstacle;
		    fTestY = shooter->y + fEyeY * fDistanceToObstacle;

            // Test if ray is out of bounds
		    if (fTestX < 0 || fTestX >= configs->mapWidth || fTestY < 0 || fTestY >= configs->mapHeight)
		    {
			    bHitWall = true;
            }
            else // Ray is inbounds so test to see if the ray cell is a wall block
			    if (configs->map[(int)fTestX][(int)fTestY])
		    {
				// Ray has hit wall
				bHitWall = true;
            }
            else 
	            if (cross(shooter->x, shooter->y, fTestX, fTestY,
		          	    target->leftFront_pos.x, target->leftFront_pos.y,
		          	    target->rightFront_pos.x, target->rightFront_pos.y, fTestX, fTestY) ||
		            cross(shooter->x, shooter->y, fTestX, fTestY,
			            target->leftFront_pos.x, target->leftFront_pos.y,
			            target->leftBack_pos.x, target->leftBack_pos.y, fTestX, fTestY) ||
		            cross(shooter->x, shooter->y, fTestX, fTestY,
		             	target->leftBack_pos.x, target->leftBack_pos.y,
		              	target->rightBack_pos.x, target->rightBack_pos.y, fTestX, fTestY) ||
	               	cross(shooter->x, shooter->y, fTestX, fTestY,
	               		target->rightBack_pos.x, target->rightBack_pos.y,
	               		target->rightFront_pos.x, target->rightFront_pos.y, fTestX, fTestY))
                    {
						bHitPlayer = true;
					}
            }
    }
    return bHitPlayer;
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
                // Update player location
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
            case SHOOTING:  
            {
                // Update player location
                size_t player_index = (*iter).first;
                Player* shooter = players_locations[player_index];
                memcpy(&shooter->x, packet.packet_data, sizeof(shooter->x));
                memcpy(&shooter->y, packet.packet_data + sizeof(shooter->x), sizeof(shooter->y));
                memcpy(&shooter->rotation, packet.packet_data + sizeof(shooter->x) + sizeof(shooter->y),
                    sizeof(shooter->rotation));

                // Check all players for a hit
                for (auto iter : players_locations)
                {
                    if (checkShouting(shooter, iter.second) && shooter->index != iter.first)
                    {
                        sendShoutingInfo(shooter, iter.second);
                        sendMovementPackets(iter.second);
                        updateMap(iter.second);
                    }
                }

                delete[] packet.packet_data;
                break;
            }
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

// Sends shouting information: decreasing of health and small movement (0.05) or return player to spawn
void sendShoutingInfo(Player *shooter, Player* player_to_change)
{
    if (player_to_change->health - configs->shootingDamage <= 0){
        player_to_change->RandomPosition(&configs); // respawn
        player_to_change->health = 100; 
    }
    else {
        player_to_change->health -= configs->shootingDamage;

        // do some moving of the target
        float fDirectX = sinf(shooter->rotation);   // Unit vector for ray in shooting direction
        float fDirectY = cosf(shooter->rotation);

        player_to_change->x += fDirectX * 0.05;
        player_to_change->y += fDirectY * 0.05;
    }

    Packet packet;
    packet.packet_type = SHOOTING;
    packet.size_of_packet_data = sizeof(player_to_change->health) + sizeof(player_to_change->x) +
     sizeof(player_to_change->y) + sizeof(player_to_change->rotation);

    packet.packet_data = new char[packet.size_of_packet_data];
    memcpy(packet.packet_data, &player_to_change->health, sizeof(player_to_change->health));
    memcpy(packet.packet_data + sizeof(player_to_change->health), &player_to_change->x, sizeof(player_to_change->x));
    memcpy(packet.packet_data + sizeof(player_to_change->health) + sizeof(player_to_change->x), 
        &player_to_change->y, sizeof(player_to_change->y));
    memcpy(packet.packet_data + sizeof(player_to_change->health) + sizeof(player_to_change->x) + sizeof(player_to_change->y), 
        &player_to_change->rotation, sizeof(player_to_change->rotation));

    size_t packet_size = sizeof(packet.packet_type) + sizeof(packet.size_of_packet_data) + packet.size_of_packet_data;
    char* packet_data = new char[packet_size];
    
    packet.serialize(packet_data);
    
    NetworkServices::sendMessage(network->sessions[player_to_change->index], packet_data, packet_size);
    delete[] packet.packet_data;
    delete[] packet_data;
}

bool cross(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float& x, float& y)
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