


#include "ClientGame.h"
#include "ClientNetwork.h"
#include "Configs.h"
#include "Player.h"
#include "Rendering.h"
#include <iostream>



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

		while (!client->gotConfigs)
			client->update();

		system("PAUSE");
	}

	Configs conf;

	std::vector<std::vector<bool>> map = conf.map;
	Player player(conf); 
	player.RandomPosition(conf);

	if (client)
		client->RegisterPlayer(player);


	// Create Screen Buffer
	wchar_t* screen = new wchar_t[conf.screenWidth * conf.screenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	auto tp1 = std::chrono::system_clock::now();
	auto tp2 = std::chrono::system_clock::now();

	auto lastShootTime = std::chrono::system_clock::now();;

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

		Rendering::CalculatePosition(player, fElapsedTime, map, tp1, lastShootTime, client);
		if (isMultiplayer)
			Rendering::RenderFrame(conf, player, conf.map, screen, hConsole, dwBytesWritten, tp1, tp2, fElapsedTime, &client->other_players);
		else 
			Rendering::RenderFrame(conf, player, conf.map, screen, hConsole, dwBytesWritten, tp1, tp2, fElapsedTime);

	}
	return 0;
}


//CONSOLE_FONT_INFOEX cfi;
//cfi.cbSize = sizeof(cfi);
//cfi.nFont = 0;
//cfi.dwFontSize.X = 4;                   // Width of each character in the font
//cfi.dwFontSize.Y = 0;			          // Height
//cfi.FontFamily = FF_DONTCARE;
//cfi.FontWeight = FW_NORMAL;
//wcscpy_s(cfi.FaceName, L"Consolas"); // Choose your font
//SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);