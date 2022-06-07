#pragma once
#include <string.h>

constexpr size_t MAX_PACKET_SIZE = 1000000;

enum PacketTypes {

    INIT_CONNECTION = 0,

    ACTION_EVENT = 1,

};


struct Packet 
{

    PacketTypes packet_type;

    // Serialize function is used to convert the packet_type data 
    // into bytes that we can send over the network
    void serialize(char* data) {
        memcpy(data, this, sizeof(Packet));
    }

    // deserialize function is used to convert bytes received over 
    // the network back into packet_type data that we can interpret.
    void deserialize(char* data) {
        memcpy(this, data, sizeof(Packet));
    }
};