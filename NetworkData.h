#pragma once
#include <string.h>

constexpr size_t MAX_PACKET_SIZE = 1000000;

enum PacketTypes 
{
    INIT_CONNECTION,

    MAP,

    MOVEMENT,

    SHOOTING,
};


struct Packet
{
    int packet_type;

    size_t size_of_packet_data;
    char* packet_data;

    //~Packet() {
    //    delete[] packet_data;
    //}

    // Serialize function is used to convert the data 
    // into bytes that we can send over the network
    void serialize(char* data) {
        memcpy(data, &packet_type, sizeof(packet_type));
        memcpy(data + sizeof(packet_type), &size_of_packet_data, sizeof(size_of_packet_data));
        memcpy(data + sizeof(packet_type) + sizeof(size_of_packet_data), packet_data, size_of_packet_data);
    }

    // deserialize function is used to convert bytes received over 
    // the network back into data that we can interpret.
    void deserialize(char* data) {
        memcpy(&packet_type, data, sizeof(packet_type));
        memcpy(&size_of_packet_data, data + sizeof(packet_type), sizeof(size_of_packet_data));
        packet_data = new char[size_of_packet_data];
        memcpy(packet_data, data + sizeof(packet_type) + sizeof(size_of_packet_data), size_of_packet_data);
    }
};