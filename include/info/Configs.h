#pragma once

#include <fstream>
#include <vector>
#include <string>



const struct Configs
{
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

	int screen_width;			// Console Screen Size X (columns)
	int screen_height;			// Console Screen Size Y (rows)
	int font_size;				// Console Font size

	int map_width;				// World Dimensions (Width)
	int map_height;				// World Dimensions (Height)

	float playerX;				// Start Position X
	float playerY;				// Start Position Y
	float playerA;				// Start Rotation
	float FOV;					// Field of View
	float depth;				// Maximum rendering distance
	float speed;				// Walking Speed

	// Multiplayer
	float max_shooting_range;
	float gun_reloading;				// Reloading in seconds
	int shooting_damage;

	float profile_width;				// Profile width
	float profile_depth;				// Profile depth
	float profile_height;				// Profile height

	int health;							// Player health 

	std::string server_ip;
	std::string default_port;

	// Map of the world
	std::vector<std::vector<bool>> map;

	// Reads configs from file or sets defaults
	Configs();

	// Gets configs from binary
	Configs(char* network_data);

	void set_server_ip();
	void set_server_ip(std::string new_ip);
	void set_port();
	void set_port(std::string new_port);

	// Gets map from binary
	void GetMap(char* map_data);

	bool ReadFromFile();
	void SaveToFile();

	void PrintAllSettings();

	void PrintMap();

	void ChangeConfig(ChangeVariant to_change, std::string new_value);
};

