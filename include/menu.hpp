/*
 *
 * File: menu.hpp
 *
 * Author: Yaroslav Kishchuk
 * Contact: kshchuk@gmail.com
 *
 */


#include <iostream>
#include <thread>

#include "network/server_game.h"
#include "info/configs.h"


void MainMenu();

void ServerLoop(network::ServerGame server)
{
	while (true)
	{
		server.update();
	}
}

void PrintMapLoop(network::ServerGame server)
{
	while (true)
	{
		server.PrintMap();
		Sleep(1000);
	}
}

void SettingsMode()
{
	system("cls");

	Configs _;

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

		Configs::PrintAllSettings();

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
			Configs::ChangeConfig(change_variant, new_value);
			Configs::SaveToFile();
			break;
		}
		case kMap:
			system("cls");
			Configs::PrintMap();
			system("PAUSE");
			break;
		case kEditMap:
		{
			system("cls");
			Configs::PrintMap();

			int elems_to_change_num;
			std::cout << "\nNumber of elements to change: ";
			std::cin >> elems_to_change_num;

			std::cout << "\nEnter the coordinates and value, where 1 - wall, 0 - space (example ' 0 23 1 ', ' 3 1 0 ' ";
			std::vector<std::vector<bool>> map = Configs::map;

			for (int i = 0; i < elems_to_change_num; i++)
			{
				int x, y, value;
				std::cin >> x >> y >> value;
				map[x][y] = value;
			}

			std::string smap;
			for (auto line : map)
				for (auto elem : line)
					smap += '1' ? elem == true : '0';

			Configs::ChangeConfig(Configs::ChangeVariant::kMap, smap);
			Configs::SaveToFile();
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

void MainMenu()
{
	system("cls");

	Configs _;

	std::cout << "\n	Main menu\n" <<
		"\n (1) - Start server" <<
		"\n (2) - Game settings" <<
		"\n (0) - Exit\n";

	enum MainMenu
	{
		kStart = 1,
		kSettings,
		kExit = 0
	};

	while (true)
	{
		int ans;
		std::cin >> ans;
		MainMenu main_menu = static_cast<MainMenu> (ans);

		switch (main_menu)
		{
		case kStart:
		{
			Configs _;

			network::ServerGame server;
			std::thread main(ServerLoop, server);

			std::cout << "\n See server logs (l) or see map (m): ";
			char ans_c;
			std::cin >> ans_c;
			if (ans_c == 'm') {
				std::thread extra(PrintMapLoop, server);
				extra.join();
			}
			main.join();
			break;
		}
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