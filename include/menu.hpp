/*
 *
 * File: menu.hpp
 *
 * Author: Yaroslav Kishchuk
 * Contact: kshchuk@gmail.com
 *
 */


#pragma once

#include <stdlib.h>

#include <iostream>
#include <chrono>
#include <tuple>

#include "../include/network/client_game.h"
#include "../include/network/client_network.h"
#include "../include/info/configs.h"
#include "../include/info/player.h"
#include "../include/info/textures.h"
#include "../include/rendering.h"
#include "../include/menu.hpp"


void MainMenu();
void SetConsoleDimestions(int, int, int);
void Pause(Configs&);
void SoloMode();
void ServerMode();
void SettingsMode();
void EditConnectionSettings(Configs&);


void MainMenu()
{
	system("cls");

	SetConsoleDimestions(60, 25, 16);

	std::cout << "\n\n		Main menu\n" <<
		"\n	(1) - Solo game (explore map)" <<
		"\n	(2) - Server game (connect to the server)"
		"\n	(3) - Settings" <<
	  "\n\n	(0) - Exit\n";

	enum MainMenu
	{
		kSolo = 1,
		kServer,
		kSettings,
		kExit = 0
	};

	while (true)
	{
		int ans;
		std::cin >> ans;
		MainMenu menu = static_cast<MainMenu> (ans);

		switch (menu)
		{
		case kSolo:
			SoloMode();
			break;
		case kServer:
			ServerMode();
			break;
		case kSettings:
			SettingsMode();
			break;
		case kExit:
			exit(0);
			break;
		default:
			continue;
			break;
		}
	}
}

void SoloMode()
{
	Textures textures;
	Configs configs;

	// Create Screen Buffer
	wchar_t* screen = new wchar_t[configs.screen_width * configs.screen_height];
	HANDLE console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(console);
	DWORD bytes_written = 0;

	SetConsoleDimestions(configs.screen_width, configs.screen_height, configs.font_size);

	auto tp1 = std::chrono::system_clock::now();
	auto tp2 = std::chrono::system_clock::now();
	auto last_firing_time = std::chrono::system_clock::now();

	std::chrono::duration<float> elapsed_time;
	float felapsed_time;

	Player* player = new Player(configs);
	network::ClientGame* client = nullptr;

	rendering::ConsoleInfo console_info(console, bytes_written, screen, tp1, tp2);
	rendering::GamingInfo game_info(felapsed_time, configs, player, last_firing_time, client);

	while (true)
	{
		// Handle pause button - ESC
		if (GetAsyncKeyState(static_cast<unsigned short> (VK_ESCAPE)) & 0x8000)
			Pause(configs);

		// We'll need time differential per frame to calculate modification
		// to movement speeds, to ensure consistant movement, as ray-tracing
		// is non-deterministic

		tp2 = std::chrono::system_clock::now();
		elapsed_time = tp2 - tp1;
		tp1 = tp2;
		felapsed_time = elapsed_time.count();

		console_info = std::tie(console, bytes_written, screen, tp1, tp2);
		game_info = std::tie(felapsed_time, configs, player, last_firing_time, client);

		rendering::CalculatePosition(game_info);
		rendering::RenderFrame(game_info, textures, console_info);
	}
}

void ServerMode()
{
	Configs configs;

	system("cls");

	EditConnectionSettings(configs);

	network::ClientGame* client = new network::ClientGame(&configs);

	while (!client->got_configs_ || !client->got_map_)
		client->update();

	Player* player = new Player(configs);
	client->RegisterPlayer(player);

	// Create Screen Buffer
	wchar_t* screen = new wchar_t[configs.screen_width * configs.screen_height];
	HANDLE console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(console);
	DWORD bytes_written = 0;

	SetConsoleDimestions(configs.screen_width, configs.screen_height, configs.font_size);

	auto tp1 = std::chrono::system_clock::now();
	auto tp2 = std::chrono::system_clock::now();
	auto last_firing_time = std::chrono::system_clock::now();

	std::chrono::duration<float> elapsed_time;
	float felapsed_time;

	Textures textures;

	rendering::ConsoleInfo console_info(console, bytes_written, screen, tp1, tp2);
	rendering::GamingInfo game_info(felapsed_time, configs, player, last_firing_time, client);

	while (true)
	{
		client->update();

		// Handle pause button - ESC
		if (GetAsyncKeyState(static_cast<unsigned short> (VK_ESCAPE)) & 0x8000)
			Pause(configs);

		// We'll need time differential per frame to calculate modification
		// to movement speeds, to ensure consistant movement, as ray-tracing
		// is non-deterministic

		tp2 = std::chrono::system_clock::now();
		elapsed_time = tp2 - tp1;
		tp1 = tp2;
		felapsed_time = elapsed_time.count();

		console_info = std::tie(console, bytes_written, screen, tp1, tp2);
		game_info = std::tie(felapsed_time, configs, player, last_firing_time, client);

		rendering::CalculatePosition(game_info);
		rendering::RenderFrame(game_info, textures, console_info);
	}
}

void SettingsMode()
{
	system("cls");
	SetConsoleDimestions(70, 120, 5);

	Configs configs;

	enum SettingsMenu
	{
		kChange = 1,
		kMap,
		kEditMap,
		kReturn = 0
	};

	while (true)
	{
		system("cls");

		configs.PrintAllSettings();

		std::cout << "\n\n (1) - Change settings" <<
			"\n (2) - Print map" <<
			"\n (3) - Edit map" <<
			"\n (0) - Return to Main menu\n";

		int ans;
		std::cin >> ans;

		SettingsMenu settings_menu = static_cast<SettingsMenu> (ans);

		switch (settings_menu)
		{
		case kChange:
		{
			std::cout << "\n Choose setting ID:	";
			std::cin >> ans;
			if (ans > 17 || ans < 1) {
				std::cout << "\nWrong option!\n";
				break;
			}
			Configs::ChangeVariant change_variant = static_cast<Configs::ChangeVariant> (ans);
			std::string new_value;
			std::cout << "Enter new value:	";
			std::cin >> new_value;
			configs.ChangeConfig(change_variant, new_value);
			configs.SaveToFile();
			break;
		}
		case kMap:
			system("cls");
			configs.PrintMap();
			system("PAUSE");
			break;
		case kEditMap:
		{
			system("cls");
			configs.PrintMap();

			int elems_to_change_num;
			std::cout << "\nNumber of elements to change: ";
			std::cin >> elems_to_change_num;

			std::cout << "\nEnter the coordinates and value, where 1 - wall, 0 - space (example ' 0 23 1 ', ' 3 1 0 ' ";
			for (int i = 0; i < elems_to_change_num; i++)
			{
				int x, y, value;
				std::cin >> x >> y >> value;
				configs.map[x][y] = value;
			}
			configs.SaveToFile();
			break;
		}
		case kReturn:
			MainMenu();
			break;
		default:
			continue;
			break;
		}
	}
}

void Pause(Configs& configs)
{
	system("cls");

	SetConsoleDimestions(120, 80, 16);

	std::cout << "\n			PAUSE\n" <<
		"\n	(1) - Resume game" <<
		"\n (2) - Change resolutions" <<
		"\n	(3) - Return to Main menu" <<
		"\n	(0) - Exit\n";

	enum PauseMenu
	{
		kResume = 1,
		kResolutions,
		kReturn,
		kExit = 0
	};

	int ans;
	std::cin >> ans;
	PauseMenu pause_menu = static_cast<PauseMenu> (ans);

	while (true)
	{
		switch (pause_menu)
		{
		case kResume:
			SetConsoleDimestions(configs.screen_width, configs.screen_height, configs.font_size);
			return;
		case kResolutions:
		{
			system("cls");

			std::cout << "\nConsole width:	";
			std::cin >> configs.screen_width;
			std::cout << "\nConsole height:	";
			std::cin >> configs.screen_height;
			std::cout << "\nFont size:		";
			std::cin >> configs.font_size;

			configs.SaveToFile();
			SetConsoleDimestions(configs.screen_width, configs.screen_height, configs.font_size);
			Pause(configs);
			return;
		}
		case kReturn:
			MainMenu();
			return;
		case kExit:
			exit(0);
			break;
		default:
			continue;
			break;
		}
	}
}

void SetConsoleDimestions(int screen_width, int screen_height, int font_size)
{
	CONSOLE_SCREEN_BUFFER_INFOEX consolesize;

	consolesize.cbSize = sizeof(consolesize);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	GetConsoleScreenBufferInfoEx(hConsole, &consolesize);

	COORD c;
	c.X = screen_width;
	c.Y = screen_height;
	consolesize.dwSize = c;

	consolesize.srWindow.Left = 0;
	consolesize.srWindow.Right = screen_width;
	consolesize.srWindow.Top = 0;
	consolesize.srWindow.Bottom = screen_height;

	static CONSOLE_FONT_INFOEX  fontex;
	fontex.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(hConsole, 0, &fontex);
	//fontex.FontWeight = 700;
	fontex.dwFontSize.X = font_size;
	fontex.dwFontSize.Y = 0;
	SetCurrentConsoleFontEx(hConsole, NULL, &fontex);

	SetConsoleScreenBufferInfoEx(hConsole, &consolesize);
}


void EditConnectionSettings(Configs& configs)
{
	std::cout << "\n Server ip:port :  " << configs.server_ip << ":" << configs.default_port << std::endl <<
		"\n (1) - Connect" <<
		"\n (2) - Edit\n";

	int ans;
	std::cin >> ans;

	switch (ans)
	{
	case 1:
		break;
	case 2:
	{
		configs.set_server_ip();
		configs.set_port();
		configs.SaveToFile();
	}
	default:
		break;
	}
}