/*
 *
 * File: network_data.h
 *
 * Author: Yaroslav Kishchuk
 * Contact: kshchuk@gmail.com
 *
 */


#pragma once

#include <string.h>


namespace network
{
    const int kMaxPacketSize = 1000000;

    enum class PacketTypes
    {
        // Initialization of connection.
        // Receive configs file from the server.
        kInitConnetion,

        // Get map from the server.
        kMap,

        // Send current location
        kMovement,

        // Send shooting direction and location.
        // Receive change of health and movement if got shooted.
        kShooting,
    };


    struct Packet
    {
        int packet_type;

        size_t size_of_packet_data;

        char* packet_data;


        // Serialize function is used to convert the data 
        // into bytes that we can send over the network
        void serialize(char* data) {
            memcpy(data, &packet_type, sizeof(packet_type));
            memcpy(data + sizeof(packet_type), &size_of_packet_data, sizeof(size_of_packet_data));
            memcpy(data + sizeof(packet_type) + sizeof(size_of_packet_data), packet_data, size_of_packet_data);
        }

        // Deserialize function is used to convert bytes received over 
        // the network back into data that we can interpret.
        void deserialize(char* data) {
            memcpy(&packet_type, data, sizeof(packet_type));
            memcpy(&size_of_packet_data, data + sizeof(packet_type), sizeof(size_of_packet_data));
            packet_data = new char[size_of_packet_data];
            memcpy(packet_data, data + sizeof(packet_type) + sizeof(size_of_packet_data), size_of_packet_data);
        }
    };

}