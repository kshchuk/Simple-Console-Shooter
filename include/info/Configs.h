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
static class Configs
{
private:
	// General settings

	static int screen_width_;   // Console Screen Size X (columns)
	static int screen_height_;  // Console Screen Size Y (rows)
	static int font_size_;      // Console Font size

	static int map_width_;      // World Dimensions (Width)
	static int map_height_;     // World Dimensions (Height)

	static float playerX_;      // Start Position X
	static float playerY_;      // Start Position Y
	static float playerA_;      // Start Rotation
	static float FOV_;          // Field of View
	static float depth_;        // Maximum rendering distance
	static float speed_;        // Walking Speed

	// Multiplayer settings

	static float max_shooting_range_;
	static float gun_reloading_;
	static int shooting_damage_;

	// Size of other players' silhouettes

	static float profile_width_;
	static float profile_depth_;
	static float profile_height_;

	static int health_;  // Player health 

	static char server_ip_[20];
	static char default_port_[10];

	// Map of the world for game
	static std::vector<std::vector<bool>> map_;


public:
	// Enum class to help modify Configs' fields. Each element means what field
	// we should change.
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

	// Readonly fields

	static constexpr const int& screen_width = screen_width_;
	static constexpr const int& screen_height = screen_height_;
	static constexpr const int& font_size = font_size_;

	static constexpr const int& map_width = map_width_;
	static constexpr const int& map_height = map_height_;

	static constexpr const float& playerX = playerX_;
	static constexpr const float& playerY = playerY_;
	static constexpr const float& playerA = playerA_;
	static constexpr const float& FOV = FOV_;
	static constexpr const float& depth = depth_;
	static constexpr const float& speed = speed_;

	static constexpr const float& max_shooting_range = max_shooting_range_;
	static constexpr const float& gun_reloading = gun_reloading_;
	static constexpr const int& shooting_damage = shooting_damage_;

	static constexpr const float& profile_width = profile_width_;
	static constexpr const float& profile_depth = profile_depth_;
	static constexpr const float& profile_height = profile_height_;

	static constexpr const int& health = health_;

	static constexpr const char* server_ip = server_ip_;
	static constexpr const char* default_port = default_port_;

	static constexpr const std::vector<std::vector<bool>>& map = map_;


	// Reads settings from file or sets defaults
	Configs();

	// Transforms binary data into struct
	Configs(char* binary_data);

	// Gets ip and port from user throught iostream
	static void set_server_ip();
	static void set_port();

	// Transforms binary data into map
	static void GetMap(char* binary_data);

	static void PrintAllSettings();

	static void PrintMap();

	// Changes setting into new value.
	static void ChangeConfig(ChangeVariant to_change, std::string new_value);

	// Reads information from file. Returns 'true' on success,
	// 'false' - on fail. 
	static bool ReadFromFile();
	// Writes information to file.
	static void SaveToFile();
};

