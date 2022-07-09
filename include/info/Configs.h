/*
 *
 * File: configs.h
 *
 * Author: Yaroslav Kishchuk
 * Contact: kshchuk@gmail.com
 *
 */

#pragma once

#include <fstream>
#include <vector>
#include <string>


// Struct to store and process game settings.
const struct Configs
{
	// Enum class to help modify Configs' fields. Each element means what field
	// we need to change.
	enum class ChangeVariant
	{
		kScreenWidth = 1,
		kScreenHeight,
		kFontSize,

		kMapWidth,
		kMapHeight,

		kPlayerX,
		kPlayerY,
		kPlayerA,
		kFOV,
		kDepth,
		kSpeed,

		kMaxShootingRange,
		kGunReloading,
		kShootingDamage,
		kHealth,

		kServerIp,
		kDefaultPort,

		kMap
	};

	// General settings

	int screen_width;   // Console Screen Size X (columns)
	int screen_height;  // Console Screen Size Y (rows)
	int font_size;      // Console Font size

	int map_width;      // World Dimensions (Width)
	int map_height;     // World Dimensions (Height)

	float playerX;      // Start Position X
	float playerY;      // Start Position Y
	float playerA;      // Start Rotation
	float FOV;          // Field of View
	float depth;        // Maximum rendering distance
	float speed;        // Walking Speed

	// Multiplayer settings

	float max_shooting_range;
	float gun_reloading;
	int shooting_damage;

	// Size of other players' silhouettes

	float profile_width;
	float profile_depth;
	float profile_height;

	int health;  // Player health 

	char server_ip[20];
	char default_port[10];

	// Map of the world for game
	std::vector<std::vector<bool>> map;

	// Reads settings from file or sets defaults
	Configs();

	// Transforms binary data into struct
	Configs(char* binary_data);

	// Gets ip and port from user throught iostream
	void set_server_ip();
	void set_port();

	// Transforms binary data into map
	void GetMap(char* binary_data);

	// Reads information from file. Returns 'true' on success,
	// 'false' - on fail. 
	bool ReadFromFile();
	// Writes information to file.
	void SaveToFile();

	void PrintAllSettings();

	void PrintMap();

	// Changes setting into new value.
	void ChangeConfig(ChangeVariant to_change, std::string new_value);
};

