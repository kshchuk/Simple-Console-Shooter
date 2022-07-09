/*
 *
 * File: configs.cpp
 *
 * Author: Yaroslav Kishchuk
 * Contact: kshchuk@gmail.com
 *
 */

#include "../include/info/configs.h"

#include <string>
#include <iostream>

#include <windows.h>

#include "../include/info/json.hpp"


Configs::Configs()
{
	if (ReadFromFile())
		return;
	else  // There's no config file
	{
		// defaults
		screen_width = 480;
		screen_height = 123;
		font_size = 5;

		map_width = 50;
		map_height = 50;

		playerX = 14.7f;
		playerY = 5.09f;
		playerA = 0.0f;
		FOV = 3.14159f / 4.0f;
		depth = 16.0f;
		speed = 4.0f;

		max_shooting_range = 50;
		gun_reloading = 1.0f;
		shooting_damage = 33;

		profile_width = 0.25f;
		profile_depth = 0.17f;
		profile_height = 1.0f;

		health = 100;

		server_ip = "127.0.0.1";
		default_port = "6881";

		map = {
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
			{1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1},
			{1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1},
			{1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1},
			{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
			{1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
			{1,1,0,1,1,1,1,0,0,0,0,0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,1,0,0,1,1,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,1,0,0,0,1},
			{1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,0,0,1},
			{1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1},
			{1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1},
			{1,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
			{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1},
			{1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1},
			{1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1},
			{1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1},
			{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
			{1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
			{1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1},
			{1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
			{1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
			{1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,1,1,1,0,0,1,1,1},
			{1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1},
			{1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1},
			{1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1},
			{1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}

		};

		SaveToFile();
	}
}

// Converts binary data into settings

Configs::Configs(char* binary_data)
{
	// Gets information
	memcpy(this, binary_data, sizeof(Configs) - sizeof(map)); // map is corrupted vector

	// Gets map
	int* imap = reinterpret_cast<int*> (binary_data + sizeof(Configs));

	SaveToFile();
}

// Converts binary data into map 

void Configs::GetMap(char* binary_map)
{
	map.clear();
	map.resize(map_height);
	for (auto& line : map)
		line.resize(map_width);

	for (size_t i = 0; i < map_height; i++)
		for (size_t j = 0; j < map_width; j++)
			map[i][j] = static_cast<bool> (binary_map[i * map_width + j]);

	SaveToFile();
}

// Reads settings form .json file 'configs.json' using json library.

bool Configs::ReadFromFile()
{
	std::ifstream json_file("config.json");
	if (json_file)
	{
		nlohmann::json j;
		json_file >> j;

		screen_width = j["screen_width"];
		screen_height = j["screen_height"];
		font_size = j["font_size"];

		map_width = j["map_width"];
		map_height = j["map_height"];

		playerX = j["playerX"];
		playerY = j["playerY"];
		playerA = j["playerA"];

		FOV = j["FOV"];
		depth = j["depth"];
		speed = j["speed"];

		max_shooting_range = j["max_shooting_range"];
		gun_reloading = j["gun_reloading"];
		shooting_damage = j["shooting_damage"];

		profile_width = j["profile_width"];
		profile_depth = j["profile_depth"];
		profile_height = j["profile_height"];

		health = j["health"];

		server_ip = j["server_ip"];
		default_port = j["default_port"];

		map = j["map"];

		json_file.close();
		return true;
	}
	else {
		json_file.close();
		return false;
	}
}

// Saves settings into .json file 'configs.json' using json library.

void Configs::SaveToFile()
{
	nlohmann::json j;

	j["screen_width"] = screen_width;
	j["screen_height"] = screen_height;
	j["font_size"] = font_size;

	j["map_width"] = map_width;
	j["map_height"] = map_height;

	j["playerX"] = playerX;
	j["playerY"] = playerY;
	j["playerA"] = playerA;

	j["FOV"] = FOV;
	j["depth"] = depth;
	j["speed"] = speed;

	j["max_shooting_range"] = max_shooting_range;
	j["gun_reloading"] = gun_reloading;
	j["shooting_damage"] = shooting_damage;

	j["profile_width"] = profile_width;
	j["profile_depth"] = profile_depth;
	j["profile_height"] = profile_height;

	j["health"] = health;

	j["server_ip"] = server_ip;
	j["default_port"] = default_port;

	j["map"] = map;

	std::ofstream json_file("config.json");
	json_file << j;
	json_file.close();
};

void Configs::set_server_ip()
{
	std::cout << "\n New IP: ";
	std::cin >> server_ip;
}

void Configs::set_port()
{
	std::cout << "\n New port: ";
	std::cin >> default_port;
}

void Configs::PrintAllSettings()
{
	std::cout << "\n			Game settings:" <<

		"\n\n		Screen settings:\n" <<
		"\n1. Screen width:     " << screen_width <<
		"\n2. Screen height:    " << screen_height <<
		"\n3. Font size:        " << font_size <<

		"\n\n		Map settings:\n" <<
		"\n4. Map width:    " << map_width <<
		"\n5. Map height:   " << map_height <<

		"\n\n		Player settings:\n"
		"\n6. Player default X:         " << playerX <<
		"\n7. Player default Y:         " << playerY <<
		"\n8. Player default rotation:  " << playerA <<
		"\n9. FOV:      " << FOV <<
		"\n10. Depth:   " << depth <<
		"\n11. Speed:   " << speed <<

		"\n\n		Multiplayer settings:\n" <<
		"\n12. Maximum shooting range:  " << max_shooting_range <<
		"\n13. Gun reloading (seconds): " << gun_reloading <<
		"\n14. Shooting damage:         " << shooting_damage <<
		"\n15. Starting health:         " << health <<

		"\n\n		Server settings:\n" <<
		"\n16. Server IP:    " << server_ip <<
		"\n17. Defalut port: " << default_port;
}

void Configs::PrintMap()
{
	std::cout << "\n\n			Playing MAP\n\n";
	for (auto line : map)
	{
		std::cout << std::endl;
		for (auto elem : line)
			std::cout << (elem ? "#" : " ");
	}
	std::cout << std::endl;
}

void Configs::ChangeConfig(ChangeVariant to_change, std::string new_value)
{
	switch (to_change)
	{
	case Configs::ChangeVariant::kScreenWidth:
		screen_width = std::stoi(new_value);
		break;
	case Configs::ChangeVariant::kScreenHeight:
		screen_height = std::stoi(new_value);
		break;
	case Configs::ChangeVariant::kFontSize:
		font_size = std::stoi(new_value);
		break;
	case Configs::ChangeVariant::kMapWidth:
		map_width = std::stoi(new_value);
		break;
	case Configs::ChangeVariant::kMapHeight:
		map_height = std::stoi(new_value);
		break;
	case Configs::ChangeVariant::kPlayerX:
		playerX = std::stof(new_value);
		break;
	case Configs::ChangeVariant::kPlayerY:
		playerY = std::stof(new_value);
		break;
	case Configs::ChangeVariant::kPlayerA:
		playerA = std::stof(new_value);
		break;
	case Configs::ChangeVariant::kFOV:
		FOV = std::stof(new_value);
		break;
	case Configs::ChangeVariant::kDepth:
		depth = std::stof(new_value);
		break;
	case Configs::ChangeVariant::kSpeed:
		speed = std::stof(new_value);
		break;
	case Configs::ChangeVariant::kMaxShootingRange:
		max_shooting_range = std::stof(new_value);
		break;
	case Configs::ChangeVariant::kGunReloading:
		gun_reloading = std::stof(new_value);
		break;
	case Configs::ChangeVariant::kShootingDamage:
		shooting_damage = std::stoi(new_value);
		break;
	case Configs::ChangeVariant::kHealth:
		health = std::stoi(new_value);
		break;
	case Configs::ChangeVariant::kServerIp:
		server_ip = new_value;
		break;
	case Configs::ChangeVariant::kDefaultPort:
		default_port = new_value;
		break;
	case Configs::ChangeVariant::kMap:
		for (int i = 0; i < map_height; i++)
			for (int j = 0; j < map_width; j++)
				map[i][j] = new_value[i * map_width + j] == '1';
		break;
	default:
		break;
	}
}
