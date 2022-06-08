
#include "ClientGame.h"
#include "Configs.h"
#include "Player.h"
#include "Rendering.h"



int main()
{
	ClientGame* client = new ClientGame();

	while (true) {
		client->update();
	}

	Configs conf;

	system("PAUSE");

	std::vector<std::vector<bool>> map = conf.map;
	size_t players_number = 1;
	Player player(conf); 
	player.RandomPosition(conf);


	// Create Screen Buffer
	wchar_t* screen = new wchar_t[conf.screenWidth * conf.screenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	auto tp1 = std::chrono::system_clock::now();
	auto tp2 = std::chrono::system_clock::now();

	while (true)
	{
		// We'll need time differential per frame to calculate modification
		// to movement speeds, to ensure consistant movement, as ray-tracing
		// is non-deterministic

		tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = elapsedTime.count();

		Rendering::CalculatePosition(player, fElapsedTime, map);
		Rendering::RenderFrame(conf, player, players_number, conf.map, screen, hConsole, dwBytesWritten, tp1, tp2, fElapsedTime);
	}

	return 0;
}

