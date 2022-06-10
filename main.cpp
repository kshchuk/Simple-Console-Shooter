
#include "ClientGame.h"
#include "ClientNetwork.h"
#include "Configs.h"
#include "Player.h"
#include "Rendering.h"
#include <iostream>



int main()
{
	std::cout << "\n	Use localhost IP? (y/n)\n";
	char* SERVER_IP;
	char ans;
	std::cin >> ans;
	if (ans == 'y')
		SERVER_IP = new char[] {"127.0.0.1"};
	else
	{
		std::cout << "\n	Server IP:	";
		std::string str;
		std::cin >> str;
		SERVER_IP = new char[str.length()];
		memcpy(SERVER_IP, str.c_str(), str.length());
		SERVER_IP[str.length()] = '\0';
	}

	ClientGame* client = new ClientGame(SERVER_IP);

	while (!client->gotConfigs)
		client->update();

	Configs conf;

	system("PAUSE");

	std::vector<std::vector<bool>> map = conf.map;
	Player player(conf); 
	player.RandomPosition(conf);
	client->RegisterPlayer(player);


	// Create Screen Buffer
	wchar_t* screen = new wchar_t[conf.screenWidth * conf.screenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	auto tp1 = std::chrono::system_clock::now();
	auto tp2 = std::chrono::system_clock::now();

	while (true)
	{
		client->update();

		// We'll need time differential per frame to calculate modification
		// to movement speeds, to ensure consistant movement, as ray-tracing
		// is non-deterministic

		tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = elapsedTime.count();

		Rendering::CalculatePosition(player, fElapsedTime, map);
		Rendering::RenderFrame(conf, player, conf.map, screen, hConsole, dwBytesWritten, tp1, tp2, fElapsedTime, client->other_players);
	}

	return 0;
}

