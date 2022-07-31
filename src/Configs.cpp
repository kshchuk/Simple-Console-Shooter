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


 // Static members initialization

int Configs::screen_width_;
int Configs::screen_height_;
int Configs::font_size_;

int Configs::map_width_;
int Configs::map_height_;

float Configs::playerX_;
float Configs::playerY_;
float Configs::playerA_;
float Configs::FOV_;
float Configs::depth_;
float Configs::speed_;

float Configs::max_shooting_range_;
float Configs::gun_reloading_;
int Configs::shooting_damage_;

float Configs::profile_width_;
float Configs::profile_depth_;
float Configs::profile_height_;

int Configs::health_;

char Configs::server_ip_[20];
char Configs::default_port_[10];

std::vector<std::vector<bool>> Configs::map_;


Configs::Configs()
{
	if (ReadFromFile())
		return;
	else  // There's no config file
	{
		// defaults
		screen_width_ = 480;
		screen_height_ = 123;
		font_size_ = 5;

		map_width_ = 50;
		map_height_ = 50;

		playerX_ = 14.7f;
		playerY_ = 5.09f;
		playerA_ = 0.0f;
		FOV_ = 3.14159f / 4.0f;
		depth_ = 16.0f;
		speed_ = 4.0f;

		max_shooting_range_ = 50;
		gun_reloading_ = 1.0f;
		shooting_damage_ = 33;

		profile_width_ = 0.25f;
		profile_depth_ = 0.17f;
		profile_height_ = 1.0f;

		health_ = 100;

		strcpy_s(server_ip_, "127.0.0.1");
		strcpy_s(default_port_, "6881");

		map_ = {
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
	memcpy(this, binary_data, sizeof(Configs) - sizeof(map_)); // map_ is corrupted vector

	// Gets map_
	int* imap = reinterpret_cast<int*> (binary_data + sizeof(Configs));

	SaveToFile();
}

// Converts binary data into map_ 

void Configs::GetMap(char* binary_map)
{
	map_.clear();
	map_.resize(map_height_);
	for (auto& line : map_)
		line.resize(map_width_);

	for (size_t i = 0; i < map_height_; i++)
		for (size_t j = 0; j < map_width_; j++)
			map_[i][j] = static_cast<bool> (binary_map[i * map_width_ + j]);

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

		screen_width_ = j["screen_width_"];
		screen_height_ = j["screen_height_"];
		font_size_ = j["font_size_"];

		map_width_ = j["map_width_"];
		map_height_ = j["map_height_"];

		playerX_ = j["playerX_"];
		playerY_ = j["playerY_"];
		playerA_ = j["playerA_"];

		FOV_ = j["FOV_"];
		depth_ = j["depth_"];
		speed_ = j["speed_"];

		max_shooting_range_ = j["max_shooting_range_"];
		gun_reloading_ = j["gun_reloading_"];
		shooting_damage_ = j["shooting_damage_"];

		profile_width_ = j["profile_width_"];
		profile_depth_ = j["profile_depth_"];
		profile_height_ = j["profile_height_"];

		health_ = j["health_"];

		std::string tmp = j["server_ip_"];
		strcpy_s(server_ip_, tmp.c_str());
		tmp = j["default_port_"];
		strcpy_s(default_port_, tmp.c_str());

		map_ = j["map_"];

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

	j["screen_width_"] = screen_width_;
	j["screen_height_"] = screen_height_;
	j["font_size_"] = font_size_;

	j["map_width_"] = map_width_;
	j["map_height_"] = map_height_;

	j["playerX_"] = playerX_;
	j["playerY_"] = playerY_;
	j["playerA_"] = playerA_;

	j["FOV_"] = FOV_;
	j["depth_"] = depth_;
	j["speed_"] = speed_;

	j["max_shooting_range_"] = max_shooting_range_;
	j["gun_reloading_"] = gun_reloading_;
	j["shooting_damage_"] = shooting_damage_;

	j["profile_width_"] = profile_width_;
	j["profile_depth_"] = profile_depth_;
	j["profile_height_"] = profile_height_;

	j["health_"] = health_;

	j["server_ip_"] = server_ip_;
	j["default_port_"] = default_port_;

	j["map_"] = map_;

	std::ofstream json_file("config.json");
	json_file << j;
	json_file.close();
};

void Configs::set_server_ip()
{
	std::cout << "\n New IP: ";
	std::cin >> server_ip_;
}

void Configs::set_port()
{
	std::cout << "\n New port: ";
	std::cin >> default_port_;
}

void Configs::PrintAllSettings()
{
	std::cout << "\n			Game settings:" <<

		"\n\n		Screen settings:\n" <<
		"\n1. Screen width:     " << screen_width_ <<
		"\n2. Screen height:    " << screen_height_ <<
		"\n3. Font size:        " << font_size_ <<

		"\n\n		Map settings:\n" <<
		"\n4. Map width:    " << map_width_ <<
		"\n5. Map height:   " << map_height_ <<

		"\n\n		Player settings:\n"
		"\n6. Player default X:         " << playerX_ <<
		"\n7. Player default Y:         " << playerY_ <<
		"\n8. Player default rotation:  " << playerA_ <<
		"\n9. FOV_:      " << FOV_ <<
		"\n10. depth_:   " << depth_ <<
		"\n11. speed_:   " << speed_ <<

		"\n\n		Multiplayer settings:\n" <<
		"\n12. Maximum shooting range:  " << max_shooting_range_ <<
		"\n13. Gun reloading (seconds): " << gun_reloading_ <<
		"\n14. Shooting damage:         " << shooting_damage_ <<
		"\n15. Starting health_:         " << health_ <<

		"\n\n		Server settings:\n" <<
		"\n16. Server IP:    " << server_ip_ <<
		"\n17. Defalut port: " << default_port_;
}

void Configs::PrintMap()
{
	std::cout << "\n\n			Playing MAP\n\n";
	for (auto line : map_)
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
		screen_width_ = std::stoi(new_value);
		break;
	case Configs::ChangeVariant::kScreenHeight:
		screen_height_ = std::stoi(new_value);
		break;
	case Configs::ChangeVariant::kFontSize:
		font_size_ = std::stoi(new_value);
		break;
	case Configs::ChangeVariant::kMapWidth:
		map_width_ = std::stoi(new_value);
		break;
	case Configs::ChangeVariant::kMapHeight:
		map_height_ = std::stoi(new_value);
		break;
	case Configs::ChangeVariant::kPlayerX:
		playerX_ = std::stof(new_value);
		break;
	case Configs::ChangeVariant::kPlayerY:
		playerY_ = std::stof(new_value);
		break;
	case Configs::ChangeVariant::kPlayerA:
		playerA_ = std::stof(new_value);
		break;
	case Configs::ChangeVariant::kFOV:
		FOV_ = std::stof(new_value);
		break;
	case Configs::ChangeVariant::kDepth:
		depth_ = std::stof(new_value);
		break;
	case Configs::ChangeVariant::kSpeed:
		speed_ = std::stof(new_value);
		break;
	case Configs::ChangeVariant::kMaxShootingRange:
		max_shooting_range_ = std::stof(new_value);
		break;
	case Configs::ChangeVariant::kGunReloading:
		gun_reloading_ = std::stof(new_value);
		break;
	case Configs::ChangeVariant::kShootingDamage:
		shooting_damage_ = std::stoi(new_value);
		break;
	case Configs::ChangeVariant::kHealth:
		health_ = std::stoi(new_value);
		break;
	case Configs::ChangeVariant::kServerIp:
		strcpy_s(server_ip_, new_value.c_str());
		break;
	case Configs::ChangeVariant::kDefaultPort:
		strcpy_s(default_port_, new_value.c_str());
		break;
	case Configs::ChangeVariant::kMap:
		for (int i = 0; i < map_height_; i++)
			for (int j = 0; j < map_width_; j++)
				map_[i][j] = new_value[i * map_width_ + j] == '1';
		break;
	default:
		break;
	}
}
