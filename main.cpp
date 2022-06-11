#include "ServerGame.h"
#include "Configs.h"
#include <thread>

void ServerLoop(ServerGame* server) {
    while (true)
    {
        server->update();
    }
}

void PrintMapLoop(ServerGame* server) {
    while (true) 
    {
        server->printMap();
        Sleep(1000);
    }
}

int main()
{
    Configs* conf = new Configs;   // Create configs file

	ServerGame* server = new ServerGame(conf);

    while (true)
    {
        server->update();
    }
    
    std::thread two(PrintMapLoop, server);
    std::thread one(ServerLoop, server);
    one.join();
    //two.join();

}