/*
* -------------------------------------------------------- Simple Console Shooter ----------------------------------------------------- *
* 
* 
* 
*/

#include <iostream>

#include "include/network/client_game.h"
#include "include/network/client_network.h"
#include "include/info/configs.h"
#include "include/info/player.h"
#include "include/info/textures.h"
#include "include/rendering.h"


//void GetInfo(bool &isMultiplayer)

int main()
{
	bool isMultiplayer;

	std::cout << "\n\n		Multiplayer (m)/Solo (s)\n\n    ";
	char ans;
	std::cin >> ans; 
	ans == 'm' ? isMultiplayer = true : isMultiplayer = false;

	ClientGame* client = nullptr;;
	char* SERVER_IP = new char[] {"127.0.0.1"};

	if (isMultiplayer)
	{
		std::cout << "\n	Use localhost IP? (y/n)\n\n     ";

		std::cin >> ans;
		if (ans == 'n')
		{
			std::cout << "\n	Server IP:	";
			std::string str;
			std::cin >> str;
			SERVER_IP = new char[str.length()];
			memcpy(SERVER_IP, str.c_str(), str.length());
			SERVER_IP[str.length()] = '\0';
		}

		client = new ClientGame(SERVER_IP, isMultiplayer);

		while (!client->gotConfigs || !client->gotMap)
			client->update();

	}
	system("PAUSE");

	Configs conf;

	Player player(conf); 
	player.RandomPosition(conf);

	if (client)
		client->RegisterPlayer(player);

	Textures* textures = new Textures;

	// Create Screen Buffer
	wchar_t* screen = new wchar_t[conf.screenWidth * conf.screenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	auto tp1 = std::chrono::system_clock::now();
	auto tp2 = std::chrono::system_clock::now();


	auto last_firing_time = std::chrono::system_clock::now();


	while (true)
	{
		if (isMultiplayer)
			client->update();

		// We'll need time differential per frame to calculate modification
		// to movement speeds, to ensure consistant movement, as ray-tracing
		// is non-deterministic

		tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = elapsedTime.count();

		rendering::CalculatePosition(player, conf, fElapsedTime, conf.map, client, last_firing_time);
		if (isMultiplayer)
			rendering::RenderFrame(conf, player, conf.map, screen, hConsole, dwBytesWritten, tp1, tp2,
				fElapsedTime, textures, last_firing_time, &client->other_players);
		else 
			rendering::RenderFrame(conf, player, conf.map, screen, hConsole, dwBytesWritten, tp1, tp2,
				fElapsedTime, textures, last_firing_time);

	}
	return 0;
}