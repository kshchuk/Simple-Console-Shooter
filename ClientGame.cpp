#include "ClientGame.h" 
#include "NetworkData.h"


ClientGame::ClientGame(void)
{
    network = new ClientNetwork();

    //// send init packet
    //const unsigned int packet_size = sizeof(Packet);
    //char packet_data[packet_size];

    //Packet packet;
    //packet.packet_type = INIT_CONNECTION;

    //packet.serialize(packet_data);

    //NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

//void ClientGame::sendActionPackets()
//{
//    // send action packet
//    const unsigned int packet_size = sizeof(Packet);
//    char packet_data[packet_size];
//
//    Packet packet;
//    packet.packet_type = ACTION_EVENT;
//
//    packet.serialize(packet_data);
//
//    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
//}

void ClientGame::update()
{
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

        switch (packet.packet_type) {

        case INIT_CONNECTION:

            config = new Configs(packet.packet_data);
            printf("Client received configuration file\n");
            break;

        default:

            printf("Error in packet types\n");

            break;
        }
    }
}
